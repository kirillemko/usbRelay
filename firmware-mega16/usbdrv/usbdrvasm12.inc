/* ���: usbdrvasm12.inc
 * ������: AVR USB driver
 * �����: Christian Starkjohann
 * �������: microsin.ru 
 * ���� ��������: 2004-12-29
 * ���������: 4
 * Copyright: (c) 2007 by OBJECTIVE DEVELOPMENT Software GmbH
 * ��������: GNU GPL v2 (��. License.txt) ��� ������������� (CommercialLicense.txt)
 * �������: $Id: usbdrvasm12.inc 483 2008-02-05 15:05:32Z cs $
 */

/* �� �������� ���� ����! ������ ����� �������� usbdrvasm.S, ������� ������ include
 *  ����� � ������ ����������� (��������� �� �������� �������)!
 */

/*
�������� ��������:
���� ���� �������� 12 ��� ������� ������������ ����� �������� USB. �� �������
��������� 12 ��� (�� ������������� ���������� � �� �������������� RC-����������).

��. usbdrv.h ��� ������ �������� ��������.

��������� ����� ���� ��� �������� �� ������� ����������, �� ������� ���, ���� ��
�� ������������� ���������, ��� �������! ��������� ����� �� ������ ������� ������������
��������� ������ CPU, �� � ������ ���������� ������!


��������� ����������� �������� ������������ (� ���������� ������):
������ �������                                            min max    CPUcycles
---------------------------------------------------------------------------
EOP �� OUT/SETUP �� �������� ������������� DATA0 (��� rx) 2   16     16-128
EOP �� IN �� �������� ������������� DATA0 (rx, ����� tx)  2   7.5    16-60
�� DATAx (rx) �� ACK/NAK/STALL (tx)                       2   7.5    16-60
*/

;���� ������������ ���������. ������� ����� �������! �� �������, ���� �� ������ ��������� �������!
;����� ������ ����������: 4 ����� + insn ������ = 7 max ���� ���������� ���������
;max ���������� �������� ����������: 34 ����� -> max 25 ������ ������� ����������
;max ������������� �����: [ret(2), YL, SREG, YH, shift, x1, x2, x3, cnt, x4] = 11 ����
;������ � ������� - ������������ ���������� ������ ������� � SOF.
USB_INTR_VECTOR:
;������� ���������� ���������: YL, SREG [sofError], YH, shift, x1, x2, x3, cnt
    push    YL              ;2 [35] push ������ ���� ����� �����. �� ������ ASAP
    in      YL, SREG        ;1 [37]
    push    YL              ;2 [39]
;----------------------------------------------------------------------------
; ������������� � ��������� sync:
;----------------------------------------------------------------------------
;������� sync-����� (D-) �� �������� (LSb) �� �������� (MSb) ����: 01010100 [1 = �������� (idle) = J, 0 = K]
;sync �� J �� ����� K �� ����� �������� sync -- ������������� ����� ������� ������
;������ ����� �� ����� ��������, ��������� ��� ������� IDLE ��� SE1 (== ���������)
waitForJ:
    sbis    USBIN, USBMINUS ;1 [40] ������� D- == 1
    rjmp    waitForJ        ;2
waitForK:
;��������� ��� �������� � ���� ������� 1/4 ����, ������� ������������� ������������.
    sbis    USBIN, USBMINUS
    rjmp    foundK
    sbis    USBIN, USBMINUS
    rjmp    foundK
    sbis    USBIN, USBMINUS
    rjmp    foundK
    sbis    USBIN, USBMINUS
    rjmp    foundK
    sbis    USBIN, USBMINUS
    rjmp    foundK
#if USB_COUNT_SOF
    lds     YL, usbSofCount
    inc     YL
    sts     usbSofCount, YL
#endif  /* USB_COUNT_SOF */
    rjmp    sofError
foundK:
;{3, 5} ����� ����� �� D- ������� ��������: 4 ����� [��� ����� 4 ��� ������������� �������]
;� ��� ���� 1 ��� �������, ��������������� ��� setup, ����� ����� ������������ 
; �������. ����� � ������� - ����� �� ������ ������� sync (������� K) ���� 
; ����� ����������
    push    YH                  ;2 [2]
    lds     YL, usbInputBufOffset;2 [4]
    clr     YH                  ;1 [5]
    subi    YL, lo8(-(usbRxBuf));1 [6]
    sbci    YH, hi8(-(usbRxBuf));1 [7]

    sbis    USBIN, USBMINUS ;1 [8] ��� ����� 2 ���� K [������� 1 ����� ������� ������]
    rjmp    haveTwoBitsK    ;2 [10]
    pop     YH              ;2 [11] ������ ����������� push
    rjmp    waitForK        ;2 [13] ��� �� ��� ����� sync, ������� ��� ���
haveTwoBitsK:
;----------------------------------------------------------------------------
; ��������� �������� � �������������� ����������, ����� �� �������� ������ ����:
;----------------------------------------------------------------------------
    push    shift           ;2 [16]
    push    x1              ;2 [12]
    push    x2              ;2 [14]

    in      x1, USBIN       ;1 [17] <-- �������� ��� 0
    ldi     shift, 0xff     ;1 [18]
    bst     x1, USBMINUS    ;1 [19]
    bld     shift, 0        ;1 [20]
    push    x3              ;2 [22]
    push    cnt             ;2 [24]
    
    in      x2, USBIN       ;1 [25] <-- �������� ��� 1
    ser     x3              ;1 [26] [����������� ���������� �������������]
    eor     x1, x2          ;1 [27]
    bst     x1, USBMINUS    ;1 [28]
    bld     shift, 1        ;1 [29]
    ldi     cnt, USB_BUFSIZE;1 [30] [����������� ���������� �������������]
    rjmp    rxbit2          ;2 [32]

;----------------------------------------------------------------------------
; ���� ��������� (����� � ������� - ����� ������ ����� ����� ����������)
;----------------------------------------------------------------------------

unstuff0:               ;1 (������ �������)
    andi    x3, ~0x01   ;1 [15]
    mov     x1, x2      ;1 [16] x2 �������� ��������� ��������� (stuffed) ���
    in      x2, USBIN   ;1 [17] <-- �������� ��� 1 �����
    ori     shift, 0x01 ;1 [18]
    rjmp    didUnstuff0 ;2 [20]

unstuff1:               ;1 (������ �������)
    mov     x2, x1      ;1 [21] x1 �������� ��������� ��������� (stuffed) ���
    andi    x3, ~0x02   ;1 [22]
    ori     shift, 0x02 ;1 [23]
    nop                 ;1 [24]
    in      x1, USBIN   ;1 [25] <-- �������� ��� 2 �����
    rjmp    didUnstuff1 ;2 [27]

unstuff2:               ;1 (������ �������)
    andi    x3, ~0x04   ;1 [29]
    ori     shift, 0x04 ;1 [30]
    mov     x1, x2      ;1 [31] x2 �������� ��������� ��������� (stuffed) ���
    nop                 ;1 [32]
    in      x2, USBIN   ;1 [33] <-- ������� ���� 3
    rjmp    didUnstuff2 ;2 [35]

unstuff3:               ;1 (������ �������)
    in      x2, USBIN   ;1 [34] <-- ������� stuffed ���� 3 [���� ���� ������� ������]
    andi    x3, ~0x08   ;1 [35]
    ori     shift, 0x08 ;1 [36]
    rjmp    didUnstuff3 ;2 [38]

unstuff4:               ;1 (������ �������)
    andi    x3, ~0x10   ;1 [40]
    in      x1, USBIN   ;1 [41] <-- ������� ������������ ���� 4
    ori     shift, 0x10 ;1 [42]
    rjmp    didUnstuff4 ;2 [44]

unstuff5:               ;1 (������ �������)
    andi    x3, ~0x20   ;1 [48]
    in      x2, USBIN   ;1 [49] <-- ������� ������������ ���� 5
    ori     shift, 0x20 ;1 [50]
    rjmp    didUnstuff5 ;2 [52]

unstuff6:               ;1 (������ �������)
    andi    x3, ~0x40   ;1 [56]
    in      x1, USBIN   ;1 [57] <-- ������� ������������ ���� 6
    ori     shift, 0x40 ;1 [58]
    rjmp    didUnstuff6 ;2 [60]

; �������������� ��������, ����������� �� ������� ����������:
; bit 0:    ����������, ������ [SE0 ����� ��������� ��-�� ���������� ��� � �����]
; bit 1:    �������� se0 
; bit 2:    �������� ������������
; bit 3:    �������������� �� �������� [������ ���� 0 �������� ������� ����� �������]
; bit 4:    ������
; bit 5:    ������
; bit 6:    ������
; bit 7:    �������, eor
rxLoop:
    eor     x3, shift   ;1 [0] �������������: x3 0 � ����������������� ����, ������� �� ��������, 1 ��� ������
    in      x1, USBIN   ;1 [1] <-- ������� ���� 0
    st      y+, x3      ;2 [3] ���������� ������
    ser     x3          ;1 [4]
    nop                 ;1 [5]
    eor     x2, x1      ;1 [6]
    bst     x2, USBMINUS;1 [7]
    bld     shift, 0    ;1 [8]
    in      x2, USBIN   ;1 [9] <-- ������� ���� 1 (��� �������� stuffed-��� 0)
    andi    x2, USBMASK ;1 [10]
    breq    se0         ;1 [11] SE0 �������� ��� ���� 1
    andi    shift, 0xf9 ;1 [12]
didUnstuff0:
    breq    unstuff0    ;1 [13]
    eor     x1, x2      ;1 [14]
    bst     x1, USBMINUS;1 [15]
    bld     shift, 1    ;1 [16]
rxbit2:
    in      x1, USBIN   ;1 [17] <-- ������� ���� 2 (��� �������� stuffed-��� 1)
    andi    shift, 0xf3 ;1 [18]
    breq    unstuff1    ;1 [19] do remaining work for bit 1
didUnstuff1:
    subi    cnt, 1      ;1 [20]
    brcs    overflow    ;1 [21] loop control
    eor     x2, x1      ;1 [22]
    bst     x2, USBMINUS;1 [23]
    bld     shift, 2    ;1 [24]
    in      x2, USBIN   ;1 [25] <-- ������� ���� 3 (��� �������� stuffed-��� 2)
    andi    shift, 0xe7 ;1 [26]
    breq    unstuff2    ;1 [27]
didUnstuff2:
    eor     x1, x2      ;1 [28]
    bst     x1, USBMINUS;1 [29]
    bld     shift, 3    ;1 [30]
didUnstuff3:
    andi    shift, 0xcf ;1 [31]
    breq    unstuff3    ;1 [32]
    in      x1, USBIN   ;1 [33] <-- ������� ���� 4
    eor     x2, x1      ;1 [34]
    bst     x2, USBMINUS;1 [35]
    bld     shift, 4    ;1 [36]
didUnstuff4:
    andi    shift, 0x9f ;1 [37]
    breq    unstuff4    ;1 [38]
    nop2                ;2 [40]
    in      x2, USBIN   ;1 [41] <-- ������� ���� 5
    eor     x1, x2      ;1 [42]
    bst     x1, USBMINUS;1 [43]
    bld     shift, 5    ;1 [44]
didUnstuff5:
    andi    shift, 0x3f ;1 [45]
    breq    unstuff5    ;1 [46]
    nop2                ;2 [48]
    in      x1, USBIN   ;1 [49] <-- ������� ���� 6
    eor     x2, x1      ;1 [50]
    bst     x2, USBMINUS;1 [51]
    bld     shift, 6    ;1 [52]
didUnstuff6:
    cpi     shift, 0x02 ;1 [53]
    brlo    unstuff6    ;1 [54]
    nop2                ;2 [56]
    in      x2, USBIN   ;1 [57] <-- ������� ���� 7
    eor     x1, x2      ;1 [58]
    bst     x1, USBMINUS;1 [59]
    bld     shift, 7    ;1 [60]
didUnstuff7:
    cpi     shift, 0x04 ;1 [61]
    brsh    rxLoop      ;2 [63] ���������� ������
unstuff7:
    andi    x3, ~0x80   ;1 [63]
    ori     shift, 0x80 ;1 [64]
    in      x2, USBIN   ;1 [65] <-- ������� stuffed-���� 7
    nop                 ;1 [66]
    rjmp    didUnstuff7 ;2 [68]

macro POP_STANDARD ; 12 ������
    pop     cnt
    pop     x3
    pop     x2
    pop     x1
    pop     shift
    pop     YH
endm
macro POP_RETI     ; 5 ������
    pop     YL
    out     SREG, YL
    pop     YL
endm

#include "asmcommon.inc"

;----------------------------------------------------------------------------
; �������� ������
;----------------------------------------------------------------------------

bitstuff0:                  ;1 (��� ���������)
    eor     x1, x4          ;1
    ldi     x2, 0           ;1
    out     USBOUT, x1      ;1 <-- �����
    rjmp    didStuff0       ;2 ����� ����� �� 2 ����� �����
bitstuff1:                  ;1 (��� ���������)
    eor     x1, x4          ;1
    rjmp    didStuff1       ;2 �� �����, ��� C ��� ����, ������� ����� ��� ����, ����� ������� OUT � �������� 0 (ror) � x2
bitstuff2:                  ;1 (��� ���������)
    eor     x1, x4          ;1
    rjmp    didStuff2       ;2 ������� ����� �� 4 ����� �����, ������ ����� � �������� 0 � x2
bitstuff3:                  ;1 (��� ���������)
    eor     x1, x4          ;1
    rjmp    didStuff3       ;2 ������� ����� ������ � ror 0 � x2
bitstuff4:                  ;1 (��� ���������)
    eor     x1, x4          ;1
    ldi     x2, 0           ;1
    out     USBOUT, x1      ;1 <-- �����
    rjmp    didStuff4       ;2 ��������� �� 2 ����� �����

sendNakAndReti:                 ;0 [-19] 19 ������ �� ��������� SOP
    ldi     x3, USBPID_NAK      ;1 [-18]
    rjmp    usbSendX3           ;2 [-16]
sendAckAndReti:                 ;0 [-19] 19 ������ �� ��������� SOP
    ldi     x3, USBPID_ACK      ;1 [-18]
    rjmp    usbSendX3           ;2 [-16]
sendCntAndReti:                 ;0 [-17] 17 ������ �� ��������� SOP
    mov     x3, cnt             ;1 [-16]
usbSendX3:                      ;0 [-16]
    ldi     YL, 20              ;1 [-15] 'x3' ��������� � R20
    ldi     YH, 0               ;1 [-14]
    ldi     cnt, 2              ;1 [-13]
;   rjmp    usbSendAndReti      �������������

; ������������ USB �������:
; idle = J
; J = (D+ = 0), (D- = 1) ��� USBOUT = 0x01
; K = (D+ = 1), (D- = 0) ��� USBOUT = 0x02
; ������������ ��������� 7.5 ��� �� EOP �� SOP ��� ������� (= 60 ������)

;usbSend:
;��������� �� ������ � 'Y'
;���������� ���� � 'cnt' -- ������� ���� sync
;������������: x1...x4, shift, cnt, Y
;����� � ������� ���������� ����� � ��� ���, ��� ������ ��� ����������������� ������� (sync pattern) ������
usbSendAndReti:             ;0 [-13] �������: 13 ������ ���� �� �������� SOP
    in      x2, USBDDR      ;1 [-12]
    ori     x2, USBMASK     ;1 [-11]
    sbi     USBOUT, USBMINUS;2 [-9] ���������� ��������� �������� (idle); D+ � D- ������ ���� � 0 (��� ����������� ���������� (pullups))
    in      x1, USBOUT      ;1 [-8] �������������� ����� ��� ����� �������� (tx)
    out     USBDDR, x2      ;1 [-7] <- ��������� ����
; �� ����� ���������������� x2 (������� bitstuff (������� ���������������� ���)), ��������� sync �������� � 0
    push    x4              ;2 [-5]
    ldi     x4, USBMASK     ;1 [-4] ����� exor (����������� ���)
    ldi     shift, 0x80     ;1 [-3] ���� sync - ������ �� ������������ ����
txLoop:                     ;       [62]
    sbrs    shift, 0        ;1 [-2] [62]
    eor     x1, x4          ;1 [-1] [63]
    out     USBOUT, x1      ;1 [0] <-- ����� ���� 0
    ror     shift           ;1 [1]
    ror     x2              ;1 [2]
didStuff0:
    cpi     x2, 0xfc        ;1 [3]
    brsh    bitstuff0       ;1 [4]
    sbrs    shift, 0        ;1 [5]
    eor     x1, x4          ;1 [6]
    ror     shift           ;1 [7]
didStuff1:
    out     USBOUT, x1      ;1 [8] <-- ����� ���� 1
    ror     x2              ;1 [9]
    cpi     x2, 0xfc        ;1 [10]
    brsh    bitstuff1       ;1 [11]
    sbrs    shift, 0        ;1 [12]
    eor     x1, x4          ;1 [13]
    ror     shift           ;1 [14]
didStuff2:
    ror     x2              ;1 [15]
    out     USBOUT, x1      ;1 [16] <-- ����� ���� 2
    cpi     x2, 0xfc        ;1 [17]
    brsh    bitstuff2       ;1 [18]
    sbrs    shift, 0        ;1 [19]
    eor     x1, x4          ;1 [20]
    ror     shift           ;1 [21]
didStuff3:
    ror     x2              ;1 [22]
    cpi     x2, 0xfc        ;1 [23]
    out     USBOUT, x1      ;1 [24] <-- ����� ���� 3
    brsh    bitstuff3       ;1 [25]
    nop2                    ;2 [27]
    ld      x3, y+          ;2 [29]
    sbrs    shift, 0        ;1 [30]
    eor     x1, x4          ;1 [31]
    out     USBOUT, x1      ;1 [32] <-- ����� ���� 4
    ror     shift           ;1 [33]
    ror     x2              ;1 [34]
didStuff4:
    cpi     x2, 0xfc        ;1 [35]
    brsh    bitstuff4       ;1 [36]
    sbrs    shift, 0        ;1 [37]
    eor     x1, x4          ;1 [38]
    ror     shift           ;1 [39]
didStuff5:
    out     USBOUT, x1      ;1 [40] <-- ����� ���� 5
    ror     x2              ;1 [41]
    cpi     x2, 0xfc        ;1 [42]
    brsh    bitstuff5       ;1 [43]
    sbrs    shift, 0        ;1 [44]
    eor     x1, x4          ;1 [45]
    ror     shift           ;1 [46]
didStuff6:
    ror     x2              ;1 [47]
    out     USBOUT, x1      ;1 [48] <-- ����� ���� 6
    cpi     x2, 0xfc        ;1 [49]
    brsh    bitstuff6       ;1 [50]
    sbrs    shift, 0        ;1 [51]
    eor     x1, x4          ;1 [52]
    ror     shift           ;1 [53]
didStuff7:
    ror     x2              ;1 [54]
    cpi     x2, 0xfc        ;1 [55]
    out     USBOUT, x1      ;1 [56] <-- ����� ���� 7
    brsh    bitstuff7       ;1 [57]
    mov     shift, x3       ;1 [58]
    dec     cnt             ;1 [59]
    brne    txLoop          ;1/2 [60/61]
;make SE0:
    cbr     x1, USBMASK     ;1 [61] ���������� SE0 [������������ �������, ��� EOP ����� ���� �� 15 �� 18 ������]
    pop     x4              ;2 [63]
;������ ����� � ������� - ���������� ������ �� ������ SE0
    out     USBOUT, x1      ;1 [0] <-- ����� SE0 -- ������ �� 2 ��� = 16 ������ ���� ���� � ��������� �������� (idle)
;2006-03-06: ���������� �������� ������ ������ � usbDeviceAddr �� ���� C � ���������:
;��������� ������ ������ ����� ����, ��� ����� ������ ���������, �� ����� ����������� (handshake)
    lds     x2, usbNewDeviceAddr;2 [2]
    lsl     x2;             ;1 [3] �� ���������� � ����� ��������� �������
    subi    YL, 20 + 2      ;1 [4] ������ ��������� ����� �� ������ ������, ��� ACK/NAK � x3
    sbci    YH, 0           ;1 [5]
    breq    skipAddrAssign  ;2 [7]
    sts     usbDeviceAddr, x2; ���� �� ���������: SE0 �� ���� ���� �������
skipAddrAssign:
;����� �������� usbDeviceAddress
    ldi     x2, 1<<USB_INTR_PENDING_BIT;1 [8] int0 ��������� �� ����� TX -- ������� ����� �������� �������
    USB_STORE_PENDING(x2)   ;1 [9]
    ori     x1, USBIDLE     ;1 [10]
    in      x2, USBDDR      ;1 [11]
    cbr     x2, USBMASK     ;1 [12] ��������� ����� ����� �� ����
    mov     x3, x1          ;1 [13]
    cbr     x3, USBMASK     ;1 [14] ������������� ���������� ����������� ���������� (pullup) �� ����� ������
    out     USBOUT, x1      ;1 [15] <-- ����� J (idle) -- ��������� SE0 (������ EOP)
    out     USBDDR, x2      ;1 [16] <-- ������ ������������ ����
    out     USBOUT, x3      ;1 [17] <-- �������� � ���, ��� ��� �������� ����������� (pull-up) ����������
    rjmp    doReturn

bitstuff5:                  ;1 (��� ���������)
    eor     x1, x4          ;1
    rjmp    didStuff5       ;2 ��� �� �����, ��� � � bitstuff1...
bitstuff6:                  ;1 (��� ���������)
    eor     x1, x4          ;1
    rjmp    didStuff6       ;2 ��� �� �����, ��� � ����...
bitstuff7:                  ;1 (��� ���������)
    eor     x1, x4          ;1
    rjmp    didStuff7       ;2 ��� �� �����, ��� � ����...
