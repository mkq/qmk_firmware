#define _BA_LAYOUT LAYOUT_ergodox_pretty(/*=======================================================================================================================================================================*/\
/* base -------|-U------------|-I------------|-A------------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|------------*/\
 KC_F1         ,LT(_FS,KC_F2) ,LT(_FD,KC_F3) ,LT(_AS,KC_F4) ,LT(_AD,KC_F5) ,KC_F6         ,CK_CYLAY        ,SPC           ,KC_F7         ,KC_F8         ,KC_F9         ,KC_F10        ,KC_F11        ,KC_F12        \
,DE_CIRC       ,KC_X          ,LT(_L5,KC_V)  ,LT(_L4,SPC)   ,LT(_L3,KC_C)  ,KC_W          ,KC_INS          ,LCAG_T(APP)   ,KC_K          ,LT(_L3,KC_H)  ,LT(_L4,KC_G)  ,LT(_L5,KC_F)  ,DE_Y          ,KC_PGUP       \
,LT(_LY,CK_SB) ,LWIN_T(KC_U)  ,LALT_T(KC_I)  ,LCTL_T(KC_A)  ,LT(_NV,KC_E)  ,KC_O                      /**/                ,KC_S          ,LT(_NV,KC_N)  ,RCTL_T(KC_R)  ,LALT_T(KC_T)  ,RWIN_T(KC_D)  ,KC_PGDN       \
,DE_PLUS       ,LT(_DW,DE_MI) ,LT(_DA,CK_QX) ,LT(_DC,KC_L)  ,KC_P          ,DE_Z          ,KC_LEAD         ,KC_DEL        ,KC_B          ,KC_M          ,DE_COMM       ,DE_DOT        ,KC_J          ,CK_DQSQ       \
,KC_LWIN       ,KC_LALT       ,KC_LCTL       ,KC_PSCR       ,BSPC                                                                        ,LT(_NV,ESC)   ,KC_UP         ,KC_DOWN       ,KC_LEFT       ,KC_RGHT       \
                                                                           ,LT(_BT,DEL)   ,KM_CUT          ,LALT_T(HOME)  ,LCTL_T(END)                                                                              \
                                                                                          ,KM_COPY         ,KC_PGUP                                                                                                 \
                                                            ,KC_LSFT       ,TAB           ,KM_PAST         ,KC_PGDN       ,LT(_L4,ENTER) ,KC_RSFT                                                                   )
#define _BT_LAYOUT LAYOUT_ergodox_pretty(/*=======================================================================================================================================================================*/\
/* base, tap only ------------|-I------------|-A------------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|------------*/\
 KC_F1         ,KC_F2         ,KC_F3         ,KC_F4         ,KC_F5         ,KC_F6         ,CK_CYLAY        ,_______       ,KC_F7         ,KC_F8         ,KC_F9         ,KC_F10        ,KC_F11        ,KC_F12        \
,_______       ,_______       ,KC_V          ,SPC           ,KC_C          ,_______       ,_______         ,APP           ,_______       ,KC_H          ,KC_G          ,KC_F          ,_______       ,_______       \
,_______       ,KC_U          ,KC_I          ,KC_A          ,KC_E          ,_______                   /**/                ,_______       ,KC_N          ,KC_R          ,KC_T          ,KC_D          ,_______       \
,_______       ,DE_MI         ,CK_QX         ,KC_L          ,_______       ,_______       ,_______         ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______                                                                     ,_______       ,_______       ,_______       ,_______       ,_______       \
                                                                           ,DEL           ,_______         ,HOME          ,END                                                                                      \
                                                                                          ,_______         ,_______                                                                                                 \
                                                            ,_______       ,_______       ,_______         ,_______       ,ENTER         ,_______                                                                   )
#define _L3_LAYOUT LAYOUT_ergodox_pretty(/*=======================================================================================================================================================================*/\
/* layer 3 ----|-U------------|-I------------|-A------------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|------------*/\
 _______       ,_______       ,_______       ,DE_DQUO       ,DE_QUOT       ,_______       ,CK_CYLAY        ,_______       ,_______       ,KC_PPLS       ,KC_PMNS       ,KC_PAST       ,KC_PSLS       ,DE_BSLS       \
,_______       ,DE_PERC       ,DE_AMPR       ,DE_LBRC       ,DE_RBRC       ,DE_GRV        ,SPC             ,SPC           ,DE_HASH       ,DE_LCBR       ,DE_RCBR       ,DE_PIPE       ,DE_SECT       ,_______       \
,LT(_LY,DE_CN) ,DE_AT         ,DE_EQL        ,DE_LABK       ,DE_RABK       ,CK_NEQ                    /**/                ,DE_ASTR       ,DE_LPRN       ,DE_RPRN       ,DE_TILD       ,DE_DLR        ,DE_SCLN       \
,DE_DOT        ,KC_0          ,KC_1          ,KC_2          ,KC_3          ,KC_4          ,BSPC            ,_______       ,KC_5          ,KC_6          ,KC_7          ,KC_8          ,KC_9          ,DE_COMM       \
,_______       ,_______       ,_______       ,_______       ,_______                                                                     ,_______       ,_______       ,_______       ,_______       ,_______       \
                                                                           ,_______       ,_______         ,_______       ,_______                                                                                  \
                                                                                          ,_______         ,_______                                                                                                 \
                                                            ,_______       ,_______       ,_______         ,_______       ,_______       ,_______                                                                   )
#define _L4_LAYOUT LAYOUT_ergodox_pretty(/*=======================================================================================================================================================================*/\
/* layer 4 ----|-U------------|-I------------|-A------------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|------------*/\
 CKC_SUP1      ,CKC_SUP2      ,CKC_SUP3      ,CKC_SUP4      ,CKC_SUP5      ,CKC_SUP6      ,CK_CYLAY        ,_______       ,CKC_SUP7      ,CKC_SUP8      ,CKC_SUP9      ,CKC_SUP0      ,CKC_SUPN      ,_______       \
,CKC_DCIRC     ,_______       ,_______       ,SPC           ,DE_ACUT       ,_______       ,_______         ,DE_PERC       ,_______       ,_______       ,_______       ,_______       ,_______       ,KC_HOME       \
,LT(_LY,SPC)   ,DE_UDIA       ,DE_BSLS       ,DE_ADIA       ,DE_EURO       ,DE_ODIA                   /**/                ,_______       ,_______       ,_______       ,_______       ,_______       ,KC_END        \
,_______       ,_______       ,_______       ,_______       ,CKC_POUND     ,DE_SS         ,_______         ,_______       ,_______       ,_______       ,_______       ,CKC_ELIP      ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______                                                                     ,_______       ,_______       ,_______       ,_______       ,_______       \
                                                                           ,_______       ,_______         ,_______       ,_______                                                                                  \
                                                                                          ,_______         ,_______                                                                                                 \
                                                            ,_______       ,_______       ,_______         ,_______       ,_______       ,SPC                                                                       )
#define _L5_LAYOUT LAYOUT_ergodox_pretty(/*=======================================================================================================================================================================*/\
/* layer 5 ----|-U------------|-I------------|-A------------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|------------*/\
 CKC_SUB1      ,CKC_SUB2      ,CKC_SUB3      ,CKC_SUB4      ,CKC_SUB5      ,CKC_SUB6      ,CK_CYLAY        ,_______       ,CKC_SUB7      ,CKC_SUB8      ,CKC_SUB9      ,CKC_SUB0      ,CKC_SUBN      ,_______       \
,CKC_DDEGR     ,CKC_DCEDI     ,CKC_DTILD     ,CKC_DDIA      ,CKC_DACUT     ,CKC_DGRV      ,_______         ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       \
,_______       ,S(DE_UDIA)    ,CKC_AEQ       ,S(DE_ADIA)    ,_______       ,S(DE_ODIA)                /**/                ,_______       ,CKC_NOT       ,CKC_COPY      ,CKC_TM        ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,CKC_POO       ,RSA(DE_SS)    ,CKC_NEQ         ,_______       ,_______       ,DE_MICR       ,_______       ,CKC_BULLET    ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______                                                                     ,_______       ,_______       ,_______       ,_______       ,_______       \
                                                                           ,_______       ,_______         ,_______       ,_______                                                                                  \
                                                                                          ,_______         ,_______                                                                                                 \
                                                            ,_______       ,_______       ,_______         ,_______       ,_______       ,_______                                                                   )
#define _NU_LAYOUT LAYOUT_ergodox_pretty(/*=======================================================================================================================================================================*/\
/* numbers & navigation ------|-I------------|-A------------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|------------*/\
 DE_BSLS       ,KC_PSLS       ,KC_PAST       ,KC_PMNS       ,KC_PPLS       ,_______       ,CK_CYLAY        ,CK_LMRES      ,_______       ,KC_PPLS       ,KC_PMNS       ,KC_PAST       ,KC_PSLS       ,DE_BSLS       \
,_______       ,DE_COLN       ,KC_7          ,KC_8          ,KC_9          ,DE_SCLN       ,KC_INS          ,KM_COPY       ,C(KC_HOME)    ,KC_HOME       ,KC_UP         ,KC_END        ,C(KC_END)     ,KC_VOLU       \
,TO(_BA)       ,DE_DOT        ,KC_4          ,KC_5          ,KC_6          ,DE_COMM                   /**/                ,KM_CUT        ,KC_LEFT       ,KC_DOWN       ,KC_RGHT       ,KC_PGUP       ,KC_VOLD       \
,_______       ,KC_0          ,KC_1          ,KC_2          ,KC_3          ,DE_PERC       ,SPC             ,_______       ,KM_COPY       ,C(KC_LEFT)    ,KC_PAST       ,C(KC_RGHT)    ,KC_PGDN       ,KC_MUTE       \
,_______       ,_______       ,_______       ,_______       ,_______                                                                     ,_______       ,KC_MSEL       ,KC_MPLY       ,KC_MPRV       ,KC_MNXT       \
                                                                           ,KC_WBAK       ,KC_WFWD         ,LALT(KC_LEFT) ,LALT(KC_RGHT)                                                                            \
                                                                                          ,_______         ,_______                                                                                                 \
                                                            ,BSPC          ,_______       ,_______         ,_______       ,_______       ,SPC                                                                       )
#define _NV_LAYOUT LAYOUT_ergodox_pretty(/*=======================================================================================================================================================================*/\
/* navigation & settings -----|-I------------|-A------------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|------------*/\
 DM_RSTP       ,DM_REC1       ,DM_REC2       ,DM_PLY1       ,DM_PLY2       ,CK_DBG        ,CK_CYLAY        ,RESET         ,RGB_TOG       ,CK_LMRES      ,KC_TAB        ,_______       ,_______       ,KC_WHOM       \
,_______       ,_______       ,_______       ,TO(_NU)       ,KC_CAPS       ,_______       ,KC_INS          ,KM_COPY       ,C(KC_HOME)    ,KC_HOME       ,KC_UP         ,KC_END        ,C(KC_END)     ,KC_VOLU       \
,_______       ,KC_LWIN       ,KC_LALT       ,KC_LCTL       ,KC_LSFT       ,_______                   /**/                ,KM_CUT        ,KC_LEFT       ,KC_DOWN       ,KC_RGHT       ,KC_PGUP       ,KC_VOLD       \
,C(DE_PLUS)    ,C(DE_MINS)    ,_______       ,KC_SLCK       ,KC_PAUS       ,_______       ,KC_DEL          ,_______       ,KM_COPY       ,C(KC_LEFT)    ,KM_PAST       ,C(KC_RGHT)    ,KC_PGDN       ,KC_MUTE       \
,_______       ,_______       ,_______       ,_______       ,_______                                                                     ,_______       ,KC_MSEL       ,KC_MPLY       ,KC_MPRV       ,KC_MNXT       \
                                                                           ,KC_WBAK       ,KC_WFWD         ,LALT(KC_LEFT) ,LALT(KC_RGHT)                                                                            \
                                                                                          ,_______         ,_______                                                                                                 \
                                                            ,KC_LSFT       ,_______       ,_______         ,_______       ,_______       ,KC_RSFT                                                                   )
#define _LY_LAYOUT LAYOUT_ergodox_pretty(/*=======================================================================================================================================================================*/\
/* layer selection -----------|-I------------|-A------------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|------------*/\
 _______       ,TO(_FS)       ,TO(_FD)       ,TO(_AS)       ,TO(_AD)       ,_______       ,_______         ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       \
,_______       ,_______       ,TO(_L5)       ,TO(_L4)       ,TO(_L3)       ,_______       ,_______         ,_______       ,_______       ,TO(_L3)       ,TO(_L4)       ,TO(_L5)       ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,TO(_NV)       ,_______                   /**/                ,_______       ,TO(_NV)       ,_______       ,_______       ,_______       ,_______       \
,_______       ,TO(_DW)       ,TO(_DA)       ,TO(_DC)       ,_______       ,_______       ,_______         ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______                                                                     ,_______       ,_______       ,_______       ,_______       ,_______       \
                                                                           ,TO(_BT)       ,_______         ,_______       ,_______                                                                                  \
                                                                                          ,_______         ,_______                                                                                                 \
                                                            ,TO(_BA)       ,_______       ,_______         ,_______       ,_______       ,TO(_BA)                                                                   )
#define _DI_LAYOUT LAYOUT_ergodox_pretty(/*=======================================================================================================================================================================*/\
/* digits -----|-U------------|-I------------|-A------------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|------------*/\
 _______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______         ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______         ,_______       ,_______       ,KC_7          ,KC_8          ,KC_9          ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______       ,_______                   /**/                ,_______       ,KC_4          ,KC_5          ,KC_6          ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______         ,_______       ,KC_0          ,KC_1          ,KC_2          ,KC_3          ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______                                                                     ,_______       ,_______       ,_______       ,_______       ,_______       \
                                                                           ,_______       ,_______         ,_______       ,_______                                                                                  \
                                                                                          ,_______         ,_______                                                                                                 \
                                                            ,_______       ,_______       ,_______         ,_______       ,_______       ,_______                                                                   )
#define _AS_LAYOUT LAYOUT_ergodox_pretty(/*=======================================================================================================================================================================*/\
/* arrows -----|-U------------|-I------------|-A------------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|------------*/\
 _______       ,_______       ,_______       ,_______       ,_______       ,_______       ,CK_CYLAY        ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       \
,_______       ,_______       ,_______       ,CKC_TRIO_N    ,_______       ,_______       ,_______         ,_______       ,_______       ,CKC_ARR_NW1   ,CKC_ARR_N1    ,CKC_ARR_NE1   ,_______       ,_______       \
,_______       ,_______       ,CKC_TRIO_W    ,CKC_TRIO_S    ,CKC_TRIO_E    ,_______                   /**/                ,_______       ,CKC_ARR_W1    ,CKC_ARR_S1    ,CKC_ARR_E1    ,CKC_ARR_WE1   ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______         ,_______       ,_______       ,CKC_ARR_SW1   ,CKC_ARR_NS1   ,CKC_ARR_SE1   ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______                                                                     ,_______       ,_______       ,_______       ,_______       ,_______       \
                                                                           ,_______       ,_______         ,_______       ,_______                                                                                  \
                                                                                          ,_______         ,_______                                                                                                 \
                                                            ,_______       ,_______       ,_______         ,_______       ,_______       ,_______                                                                   )
#define _AD_LAYOUT LAYOUT_ergodox_pretty(/*=======================================================================================================================================================================*/\
/* double arrows -------------|-I------------|-A------------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|------------*/\
 _______       ,_______       ,_______       ,_______       ,_______       ,_______       ,CK_CYLAY        ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       \
,_______       ,_______       ,_______       ,CKC_TRIF_N    ,_______       ,_______       ,_______         ,_______       ,_______       ,CKC_ARR_NW2   ,CKC_ARR_N2    ,CKC_ARR_NE2   ,_______       ,_______       \
,_______       ,_______       ,CKC_TRIF_W    ,CKC_TRIF_S    ,CKC_TRIF_E    ,_______                   /**/                ,_______       ,CKC_ARR_W2    ,CKC_ARR_S2    ,CKC_ARR_E2    ,CKC_ARR_WE2   ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______         ,_______       ,_______       ,CKC_ARR_SW2   ,CKC_ARR_NS2   ,CKC_ARR_SE2   ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______                                                                     ,_______       ,_______       ,_______       ,_______       ,_______       \
                                                                           ,_______       ,_______         ,_______       ,_______                                                                                  \
                                                                                          ,_______         ,_______                                                                                                 \
                                                            ,_______       ,_______       ,_______         ,_______       ,_______       ,_______                                                                   )
#define _FS_LAYOUT LAYOUT_ergodox_pretty(/*=======================================================================================================================================================================*/\
/* frames -----|-U------------|-I------------|-A------------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|------------*/\
 _______       ,_______       ,_______       ,_______       ,_______       ,_______       ,CK_CYLAY        ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______         ,_______       ,_______       ,CKC_FRM_NW1   ,CKC_FRM_N1    ,CKC_FRM_NE1   ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______       ,_______                   /**/                ,CKC_FRM_VL1   ,CKC_FRM_W1    ,CKC_FRM_CR1   ,CKC_FRM_E1    ,CKC_FRM_HL1   ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______         ,_______       ,_______       ,CKC_FRM_SW1   ,CKC_FRM_S1    ,CKC_FRM_SE1   ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______                                                                     ,_______       ,_______       ,_______       ,_______       ,_______       \
                                                                           ,_______       ,_______         ,_______       ,_______                                                                                  \
                                                                                          ,_______         ,_______                                                                                                 \
                                                            ,_______       ,_______       ,_______         ,_______       ,_______       ,_______                                                                   )
#define _FD_LAYOUT LAYOUT_ergodox_pretty(/*=======================================================================================================================================================================*/\
/* double frames -------------|-I------------|-A------------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|------------*/\
 _______       ,_______       ,_______       ,_______       ,_______       ,_______       ,CK_CYLAY        ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______         ,_______       ,_______       ,CKC_FRM_NW2   ,CKC_FRM_N2    ,CKC_FRM_NE2   ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______       ,_______                   /**/                ,CKC_FRM_VL2   ,CKC_FRM_W2    ,CKC_FRM_CR2   ,CKC_FRM_E2    ,CKC_FRM_HL2   ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______         ,_______       ,_______       ,CKC_FRM_SW2   ,CKC_FRM_S2    ,CKC_FRM_SE1   ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______                                                                     ,_______       ,_______       ,_______       ,_______       ,_______       \
                                                                           ,_______       ,_______         ,_______       ,_______                                                                                  \
                                                                                          ,_______         ,_______                                                                                                 \
                                                            ,_______       ,_______       ,_______         ,_______       ,_______       ,_______                                                                   )
#define TRANS_LAYOUT LAYOUT_ergodox_pretty(/*=====================================================================================================================================================================*/\
/* transparent |-U------------|-I------------|-A------------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|------------*/\
 _______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______         ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______         ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______       ,_______                   /**/                ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______         ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       ,_______       \
,_______       ,_______       ,_______       ,_______       ,_______                                                                     ,_______       ,_______       ,_______       ,_______       ,_______       \
                                                                           ,_______       ,_______         ,_______       ,_______                                                                                  \
                                                                                          ,_______         ,_______                                                                                                 \
                                                            ,_______       ,_______       ,_______         ,_______       ,_______       ,_______                                                                   )


/* ===============================================================================================================================================================================================================*/\
/* config for util/keymap-to-keyboard-layout-editor --------|-E------------|-O------------|                               |-S------------|-N------------|-R------------|-T------------|-D------------|-------------/\
#define _KLE__LAYOUT LAYOUT_ergodox_pretty(                                                                                                                                                                         \
 AA            ,AB            ,AC            ,AD            ,AE            ,AF            ,AG              ,AH            ,AI            ,AJ            ,AK            ,AL            ,AM            ,AN            \
,BA            ,BB            ,BC            ,BD            ,BE            ,BF            ,BG              ,BH            ,BI            ,BJ            ,BK            ,BL            ,BM            ,BN            \
,CA            ,CB            ,CC            ,CD            ,CE            ,CF                                            ,CG            ,CH            ,CI            ,CJ            ,CK            ,CL            \
,DA            ,DB            ,DC            ,DD            ,DE            ,DF            ,DG              ,DH            ,DI            ,DJ            ,DK            ,DL            ,DM            ,DN            \
,EA            ,EB            ,EC            ,ED            ,EE                                                                          ,EJ            ,EK            ,EL            ,EM            ,EN            \
                                                                           ,TL3           ,TL4             ,TR4           ,TR3                                                                                      \
                                                                                          ,TL5             ,TR5                                                                                                     \
                                                            ,TL1           ,TL2           ,TL6             ,TR6           ,TR2           ,TR1                                                                       )
*/

/*
Emacs| Local Variables:
Emacs| eval: (highlight-phrase "^#define\\b.*\\bLAYOUT.*" compilation-error-face)
Emacs| eval: (highlight-phrase ".*/\\*\\*/.*" compilation-warning-face)
Emacs| End:
*/