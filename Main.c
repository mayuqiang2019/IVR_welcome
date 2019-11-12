
#include <windows.h>
#include <winnt.h>
#include <limits.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#include <srllib.h>
#include <dxxxlib.h>
#include <dtilib.h>
#include <sctools.h>
#include <gclib.h>
#include <gcerr.h>
#include <gcisdn.h>

//#include <sqlcli1.h>
#include "sql.h"
#include "sqlext.h"
#include "odbcinst.h"

#define  MAIN_WND_WIDTH    810
#define  MAIN_WND_HIGHT    600
#define  CHILD_WND_WIDTH   125
#define  CHILD_WND_HIGHT   50

#define  INITIAL	0
#define  INCOMING	1
#define  OUTGOING	2
#define  ALARM		5

#define  IN_DTTM		0
#define  OUT_DTTM		1
#define  UPDATE_DTTM	2

#define  ENABLE_LOG		1
#define  LOG_INF		1
#define  LOG_SQL		2
#define	 LOG_EVT		3
#define  LOG_ERR		10
#define  ENABLE_LOG_INF		1
#define  ENABLE_LOG_SQL		0
#define  ENABLE_LOG_EVT		1
#define  ENABLE_LOG_ERR		1

#define  CALL_NOANSWER		1
#define  CALL_ENGAGE		2
#define  CALL_FAIL			3

#define  MAX_CHANS_PC		60
#define	 MAX_CHANS_ISDN		30
#define  TABLE_SIZES		500

#define GCAPI           1
#define D40             2

#define    YES   0
#define    NO    1

#define ST_RESET			1
#define ST_NULL				2
#define ST_OFFERED			3
#define ST_ANSWERCALL		5
#define ST_WELCOME			6
#define ST_DROPCALL			9


#define ST_LANGUAGE			201
#define ST_GETLANGUAGE		202
#define ST_CHKLANGUAGE		203
#define ST_WRONGLANGUAGE	204
#define ST_NOLANGUAGE		205

#define ST_SERVICE			206
#define ST_GETSERVICE		207
#define ST_CHKSERVICE		208
#define ST_WRONGSERVICE		209
#define ST_NOSERVICE		210

#define ST_ACKSMS			211
#define ST_GETACKSMS		212
#define ST_CHKACKSMS		213
#define ST_WRONGACKSMS		214
#define ST_NOACKSMS			215

#define ST_MOBILENO			220
#define ST_GETMOBILENO		221
#define ST_CHKMOBILENO		222
#define ST_REPEATMOBILE		223
#define ST_WRONGMOBILENO	224
#define ST_NOMOBILENO		225

#define ST_CONFIRMMNO		226
#define ST_CHKCONFMNO		227
#define ST_GETCONFMNO		228
#define ST_WRONGCONFMNO		229
#define ST_NOCONFMNO		230
#define ST_RECVSMS			231

#define ST_NRIC				12
#define ST_GETNRIC			16
#define ST_CHKNRIC			17
#define ST_CHKDIG			19
#define ST_CHKDTMF			21
#define ST_REPEATNRIC		22
#define ST_WRONGNRIC		23
#define ST_NONRIC			24
#define ST_N0				30
#define ST_N1				31
#define ST_N2				32
#define ST_N3				33
#define ST_N4				34
#define ST_N5				35
#define ST_N6				36
#define ST_N7				37
#define ST_N8				38
#define ST_N9				39
#define ST_N10				40
#define ST_N11				41
#define ST_N12				42
#define ST_N13				43
#define ST_N14				44
#define ST_N15				45
#define ST_N16				46
#define ST_N17				47
#define ST_N18				48
#define ST_N19				49
#define ST_N20				50
#define ST_N30				60
#define ST_N40				61
#define ST_N50				62
#define ST_N60				63
#define ST_N70				64
#define ST_N80				65
#define ST_N90				66
#define ST_N100				67
#define ST_N1000			68
#define ST_CENT				69
#define ST_CENTS			70
#define ST_DOLLAR			71
#define ST_DOLLARS			72

#define ST_AND				88

#define ST_CONFIRMDIG		90
#define ST_CHKCONFIRM		91
#define ST_GETCONFIRM		92
#define ST_WRONGCONFIRM		93
#define ST_NOTMOB			94
#define ST_ENDCALL			95
#define ST_CONNECT			107
#define ST_BRO1				108
#define ST_BRO2				109
#define ST_MACKCALL			119
#define ST_PLAYDIG			120
#define ST_MAXRETRY			122
#define ST_PROCEEDING		132
#define ST_CALLRING			133
#define ST_SYSNOUSE			134
#define ST_ACKNOWLEDGED		135
#define ST_DELETED			137
#define ST_NOCONFIRM		139
#define ST_ACK1				140
#define ST_ACK2				141
#define ST_EMPTY			143
#define ST_ALARM			144
#define ST_PLAYALARM		145
#define ST_ALARMVOICE		146
#define ST_REPEATACK		147
#define ST_GETANYDIG		148


HWND		hWnd;
HINSTANCE	hInstance;
HWND		hChildWnd[MAX_CHANS_PC + 1];
HWND		hStaticText[MAX_CHANS_PC + 1];
char		szChanMsg[MAX_CHANS_PC + 1][17];
int			msgfh[MAX_CHANS_PC + 1];
char		APP_CODE[5];
char		PC_ID[3];
char		VoiceFolder[50];
char		m_cUsername[10];
char		m_cPcname[16];
char		m_cDialdigits[15];

SQLHENV		henv;
SQLHDBC		hdbc;
HANDLE		hThread;
DWORD		dThreadid;

int  m_total_chans;
int  m_total_outgoing;
int  m_outgoing_chans;

int  g_ivrperdig;
int  g_ivrnoretry;
int  g_isdnperpc;
int  g_outgoingperisdn;

GC_MAKECALL_BLK gc_mkcall_blk;
MAKECALL_BLK mkcall_blk;
DV_TPT tpt[3];

int language_fh;
int wronglanguage_fh;
int nolanguage_fh;
int welcome_efh;
int n0_efh;
int n1_efh;
int n2_efh;
int n3_efh;
int n4_efh;
int n5_efh;
int n6_efh;
int n7_efh;
int n8_efh;
int n9_efh;
int n10_efh;
int n11_efh;
int n12_efh;
int n13_efh;
int n14_efh;
int n15_efh;
int n16_efh;
int n17_efh;
int n18_efh;
int n19_efh;
int n20_efh;
int n30_efh;
int n40_efh;
int n50_efh;
int n60_efh;
int n70_efh;
int n80_efh;
int n90_efh;
int n100_efh;
int n1000_efh;

int cent_efh;
int cents_efh;
int dollar_efh;
int dollars_efh;
int and_efh;
int service_efh;
int ic_efh;
int nric_efh;
int fin_efh;
int passport_efh;
int repeatnric_efh;
int confirm_efh;
int empty_efh;
int notmob_efh;
int endcall_efh;
int wrongservice_efh;
int wrongnric_efh;
int wrongic_efh;
int wrongfin_efh;
int wrongpassport_efh;
int wrongdigit_efh;
int maxretry_efh;
int noentry_efh;
int sysnouse_efh;
int acknowledged_efh;
int deleted_efh;
int repeat_efh;
int alarm_efh;

int welcome_cfh;
int n0_cfh;
int n1_cfh;
int n2_cfh;
int n3_cfh;
int n4_cfh;
int n5_cfh;
int n6_cfh;
int n7_cfh;
int n8_cfh;
int n9_cfh;
int n10_cfh;
int n11_cfh;
int n12_cfh;
int n13_cfh;
int n14_cfh;
int n15_cfh;
int n16_cfh;
int n17_cfh;
int n18_cfh;
int n19_cfh;
int n20_cfh;
int n30_cfh;
int n40_cfh;
int n50_cfh;
int n60_cfh;
int n70_cfh;
int n80_cfh;
int n90_cfh;
int n100_cfh;
int n1000_cfh;

int cent_cfh;
int cents_cfh;
int dollar_cfh;
int dollars_cfh;
int and_cfh;
int service_cfh;
int ic_cfh;
int nric_cfh;
int fin_cfh;
int passport_cfh;
int repeatnric_cfh;
int confirm_cfh;
int empty_cfh;
int notmob_cfh;
int endcall_cfh;
int wrongservice_cfh;
int wrongnric_cfh;
int wrongic_cfh;
int wrongfin_cfh;
int wrongpassport_cfh;
int wrongdigit_cfh;
int maxretry_cfh;
int noentry_cfh;
int sysnouse_cfh;
int acknowledged_cfh;
int deleted_cfh;
int repeat_cfh;
int alarm_cfh;
int broadcast1_efh;
int broadcast1_cfh;
int broadcast2_efh;
int broadcast2_cfh;
int ack1_efh;
int ack1_cfh;
int ack2_efh;
int ack2_cfh;
int acksms_efh;
int acksms_cfh;
int mobileno_efh;
int mobileno_cfh;
int wrongmobileno_efh;
int wrongmobileno_cfh;
int recvsms_efh;
int recvsms_cfh;

typedef struct {
	int current_state;
	int event;
	int next_state;
	void (*funcptr)(int);

}TABLE;

struct linebag{
	LINEDEV ldev;
	CRN crn;
	int voiceh;
	int timesloth;
	int state;
	int lststate;
	int nBoard;
	int nChannel;
	int in_process;
	int switched;
	int blocked;
	int flag;
	DV_DIGIT digbuf;
	DX_IOTT iott[1];
	time_t start_dttm;
	time_t connect_dttm;
	time_t end_dttm;
	int nCallresult;
	
	char cName[67];
	char cNRIC[21];
	char cPin[21];
	char cAckMobileno[21];
	int nMobid;
	int nAlarmid;
	char cAlarmcode[11];
	char cExcuseflg[2];
	int nWhichcontactno;
	int nPriority;
	char cCalltype;
	char cCalldigits[50];
	int  nWhichbroadcast;
	char cContactno[9];
	char cRegistercode[5];
	char cOutstanding[20];
	char cBrofile1[10];
	char cBrofile2[10];
	char cAckfile1[10];
	char cAckfile2[10];
	char cAckstatus;
	char cBroreset;
	char cMonth[3];
	char cDay[3];
	char cTime1[5];
	int nLanguage;
	int nICtype;
	int nAcksms;
	int nAdhocid;

	char playnumber[21];
	int playno;
	int noofretry;
	int noofpin;
	int noofreplay;
	int noofmobileno;
	
} oPort[MAX_CHANS_PC + 1];



BOOL InitApplication(HINSTANCE);
LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM); 
LRESULT WINAPI ChildProc(HWND, UINT, WPARAM, LPARAM); 
DWORD WINAPI ThreadDialogic(void *);

int getxcord(int);
int getycord(int);
int getindex(HWND);
int getindexvoice(HWND);
void sys_init(void);
void sys_exit(char *);
void sys_update_log(int, int, char *);

void wait_event();
void check_event(int, int);

void play(int);

void getdigs(int);
void dropcall(int);
void getlanguage(int);
void chklanguage(int);
void getservice(int);
void chkservice(int);
void getnric(int);
void chknric(int);
void chkdig(int);
void chkdtmf(int);
void chkconfirm(int);
void chkmobid(int);
void playlong(int);
void playdig(int);
void playalarm(int);
void chkrepeatdig(int);
void getacksms(int);
void chkacksms(int);
void getmobileno(int);
void chkmobileno(int);
void chkconfmno(int);

int cmd_open_database(void);
int cmd_close_database();
int update_station_register(int);
void update_mobnominalroll(int, int, char *, char, int);
void update_alarm_log(int, char);
int insert_broadcast_detail(int, int);
int cmd_get_parameter();
int update_audit_trace(int);
void check_alarm_log(void);
int check_broadcast_timekeeper(void);
int check_broadcast_timetable(char *);
int check_ic_type(int, char *);
void check_mobnominalroll(void);

TABLE table[TABLE_SIZES]=
    { /* current_state	event			next_stat		function */
	{ ST_ANSWERCALL,	GCEV_ANSWERED,  ST_WELCOME,		play        },
	
	{ ST_MAXRETRY,		TM_EOD,         ST_ENDCALL,		playlong    },
	{ ST_MAXRETRY,      TM_USRSTOP,     ST_DROPCALL,	dropcall    },

	{ ST_WELCOME,		TM_EOD,         ST_LANGUAGE,	play		},
	{ ST_WELCOME,		TM_MAXDTMF,     ST_LANGUAGE,	play        },
	{ ST_WELCOME,		TM_USRSTOP,     ST_DROPCALL,	dropcall    },

	{ ST_LANGUAGE,      TM_EOD,         ST_GETLANGUAGE,	getlanguage  },
	{ ST_LANGUAGE,      TM_MAXDTMF,     ST_GETLANGUAGE,  getlanguage  },
	{ ST_LANGUAGE,      TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_GETLANGUAGE,   TM_MAXDTMF,     ST_CHKLANGUAGE,	chklanguage  },
	{ ST_GETLANGUAGE,   TM_MAXTIME,     ST_CHKLANGUAGE,	chklanguage	},
	{ ST_GETLANGUAGE,   TM_DIGIT,	    ST_CHKLANGUAGE,	chklanguage	},
	{ ST_GETLANGUAGE,   TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_NOLANGUAGE,    TM_EOD,         ST_LANGUAGE,	play        },
	{ ST_NOLANGUAGE,    TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_WRONGLANGUAGE, TM_EOD,         ST_LANGUAGE,	play        },
	{ ST_WRONGLANGUAGE, TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	
	{ ST_SERVICE,       TM_EOD,         ST_GETSERVICE,	getservice  },
	{ ST_SERVICE,       TM_MAXDTMF,     ST_GETSERVICE,  getservice  },
	{ ST_SERVICE,       TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_GETSERVICE,    TM_MAXDTMF,     ST_CHKSERVICE,	chkservice  },
	{ ST_GETSERVICE,    TM_MAXTIME,     ST_CHKSERVICE,	chkservice	},
	{ ST_GETSERVICE,    TM_DIGIT,	    ST_CHKSERVICE,	chkservice	},
	{ ST_GETSERVICE,    TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_NOSERVICE,     TM_EOD,         ST_SERVICE,		play        },
	{ ST_NOSERVICE,     TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_WRONGSERVICE,  TM_EOD,         ST_SERVICE,		play        },
	{ ST_WRONGSERVICE,  TM_USRSTOP,     ST_DROPCALL,	dropcall    },
		
	{ ST_NRIC,          TM_EOD,         ST_GETNRIC,		getnric     },
	{ ST_NRIC,          TM_MAXDTMF,     ST_GETNRIC,		getnric     },
	{ ST_NRIC,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_GETNRIC,       TM_MAXDTMF,     ST_CHKNRIC,		chknric		},
	{ ST_GETNRIC,       TM_MAXTIME,     ST_CHKNRIC,		chknric		},
	{ ST_GETNRIC,       TM_DIGIT,	    ST_CHKNRIC,		chknric		},
	{ ST_GETNRIC,       TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_NONRIC,        TM_EOD,         ST_NRIC,		play        },
	{ ST_NONRIC,        TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_WRONGNRIC,     TM_EOD,         ST_NRIC,		play        },
	{ ST_WRONGNRIC,     TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	
	{ ST_REPEATNRIC,    TM_EOD,         ST_REPEATNRIC,	chkdig		},
	{ ST_REPEATNRIC,    TM_MAXDTMF,     ST_REPEATNRIC,	chkdtmf		},
	{ ST_REPEATNRIC,    TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	
	{ ST_MOBILENO,      TM_EOD,         ST_GETMOBILENO,	getmobileno },
	{ ST_MOBILENO,      TM_MAXDTMF,     ST_GETMOBILENO,	getmobileno },
	{ ST_MOBILENO,      TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_GETMOBILENO,   TM_MAXDTMF,     ST_CHKMOBILENO,	chkmobileno },
	{ ST_GETMOBILENO,   TM_MAXTIME,     ST_CHKMOBILENO, chkmobileno	},
	{ ST_GETMOBILENO,   TM_DIGIT,	    ST_CHKMOBILENO,	chkmobileno	},
	{ ST_GETMOBILENO,   TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_NOMOBILENO,    TM_EOD,         ST_MOBILENO,	play        },
	{ ST_NOMOBILENO,    TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_WRONGMOBILENO, TM_EOD,         ST_MOBILENO,	play        },
	{ ST_WRONGMOBILENO, TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	
	{ ST_REPEATMOBILE,  TM_EOD,         ST_REPEATMOBILE, chkdig		},
	{ ST_REPEATMOBILE,  TM_MAXDTMF,     ST_REPEATMOBILE, chkdtmf	},
	{ ST_REPEATMOBILE,  TM_USRSTOP,     ST_DROPCALL,	dropcall    },

	{ ST_N0,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N0,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N0,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N1,           TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N1,           TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N1,           TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N2,           TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N2,           TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N2,           TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N3,         TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N3,         TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N3,         TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N4,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N4,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N4,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N5,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N5,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N5,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N6,           TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N6,           TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N6,           TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N7,         TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N7,         TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N7,         TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N8,         TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N8,         TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N8,         TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N9,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N9,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N9,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	
	{ ST_N10,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N10,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N10,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N11,           TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N11,           TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N11,           TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N12,           TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N12,           TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N12,           TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N13,         TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N13,         TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N13,         TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N14,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N14,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N14,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N15,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N15,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N15,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N16,           TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N16,           TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N16,           TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N17,         TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N17,         TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N17,         TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N18,         TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N18,         TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N18,         TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N19,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N19,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N19,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	
	{ ST_N20,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N20,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N20,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N30,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N30,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N30,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N40,           TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N40,           TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N40,           TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N50,           TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N50,           TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N50,           TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N60,         TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N60,         TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N60,         TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N70,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N70,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N70,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N80,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N80,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N80,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N90,           TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N90,           TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N90,           TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N100,         TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N100,         TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N100,         TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_N1000,         TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_N1000,         TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_N1000,         TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_CENT,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_CENT,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_CENT,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_CENTS,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_CENTS,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_CENTS,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_DOLLAR,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_DOLLAR,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_DOLLAR,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_DOLLARS,          TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_DOLLARS,          TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_DOLLARS,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_AND,	        TM_EOD,         ST_CHKDIG,		chkdig		},
	{ ST_AND,			TM_MAXDTMF,     ST_CHKDTMF,		chkdtmf		},
	{ ST_AND,			TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	
	{ ST_CONFIRMDIG,    TM_MAXDTMF,     ST_GETCONFIRM,	getdigs		},
	{ ST_CONFIRMDIG,    TM_EOD,         ST_GETCONFIRM,	getdigs		},
	{ ST_CONFIRMDIG,    TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_GETCONFIRM,    TM_EOD,         ST_CHKCONFIRM,	chkconfirm  },
	{ ST_GETCONFIRM,    TM_MAXDTMF,     ST_CHKCONFIRM,	chkconfirm	},
	{ ST_GETCONFIRM,    TM_MAXTIME,     ST_CHKCONFIRM,	chkconfirm	},
	{ ST_GETCONFIRM,    TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_WRONGCONFIRM,  TM_EOD,         ST_REPEATNRIC,  playlong    },
	{ ST_WRONGCONFIRM,  TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_NOCONFIRM,     TM_EOD,         ST_REPEATNRIC,	playlong    },
	{ ST_NOCONFIRM,     TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	
	{ ST_CONFIRMMNO,    TM_MAXDTMF,     ST_GETCONFMNO,	getdigs		},
	{ ST_CONFIRMMNO,    TM_EOD,         ST_GETCONFMNO,	getdigs		},
	{ ST_CONFIRMMNO,    TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_GETCONFMNO,    TM_EOD,         ST_CHKCONFMNO,	chkconfmno  },
	{ ST_GETCONFMNO,    TM_MAXDTMF,     ST_CHKCONFMNO,	chkconfmno	},
	{ ST_GETCONFMNO,    TM_MAXTIME,     ST_CHKCONFMNO,	chkconfmno	},
	{ ST_GETCONFMNO,    TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_WRONGCONFMNO,  TM_EOD,         ST_REPEATMOBILE, playlong    },
	{ ST_WRONGCONFMNO,  TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_NOCONFMNO,     TM_EOD,         ST_REPEATMOBILE, playlong    },
	{ ST_NOCONFMNO,     TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	
	{ ST_ACKNOWLEDGED,  TM_EOD,         ST_DROPCALL,	dropcall    },
	{ ST_ACKNOWLEDGED,  TM_MAXDTMF,     ST_DROPCALL,	dropcall    },
	{ ST_ACKNOWLEDGED,  TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_DELETED,       TM_EOD,         ST_DROPCALL,	dropcall    },
	{ ST_DELETED,       TM_MAXDTMF,     ST_DROPCALL,	dropcall    },
	{ ST_DELETED,       TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	
	{ ST_ACK1,          TM_EOD,         ST_ACK1,		chkdig		},
	{ ST_ACK1,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	
	{ ST_ACK2,          TM_EOD,         ST_REPEATACK,	play	    },
	{ ST_ACK2,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },	
	
	{ ST_ACKSMS,	    TM_EOD,         ST_GETACKSMS,	getacksms  },
	{ ST_ACKSMS,        TM_MAXDTMF,     ST_GETACKSMS,   getacksms  },
	{ ST_ACKSMS,        TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_GETACKSMS,     TM_MAXDTMF,     ST_CHKACKSMS,	chkacksms  },
	{ ST_GETACKSMS,     TM_MAXTIME,     ST_CHKACKSMS,	chkacksms	},
	{ ST_GETACKSMS,     TM_DIGIT,	    ST_CHKACKSMS,	chkacksms	},
	{ ST_GETACKSMS,     TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_NOACKSMS,      TM_EOD,         ST_ACKSMS,		play		},
	{ ST_NOACKSMS,      TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	
	{ ST_REPEATACK,	    TM_MAXDTMF,     ST_REPEATACK,	chkrepeatdig},
	{ ST_REPEATACK,	    TM_EOD,         ST_GETANYDIG,	getdigs		},
	{ ST_REPEATACK,	    TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_GETANYDIG,     TM_MAXDTMF,     ST_REPEATACK,	chkrepeatdig},
	{ ST_GETANYDIG,     TM_MAXTIME,     ST_ENDCALL,     playlong	},
	{ ST_GETANYDIG,     TM_USRSTOP,     ST_DROPCALL,	dropcall    },	
	{ ST_ENDCALL,       TM_EOD,			ST_DROPCALL,	dropcall    },
	{ ST_ENDCALL,       TM_USRSTOP,     ST_DROPCALL,	dropcall    },	
	{ ST_NOTMOB,        TM_EOD,         ST_ENDCALL,		playlong	},
	{ ST_NOTMOB,        TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	
	{ ST_EMPTY,			TM_EOD,         ST_DROPCALL,	dropcall    },
	{ ST_EMPTY,		    TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_RECVSMS,       TM_EOD,			ST_DROPCALL,	dropcall    },
	{ ST_RECVSMS,       TM_USRSTOP,     ST_DROPCALL,	dropcall    },	
	
	{ ST_ALARM,			GCEV_CONNECTED, ST_PLAYALARM,	playalarm	},
	{ ST_PLAYALARM,     TDX_DIAL,		ST_ALARMVOICE,	play		},
	{ ST_PLAYALARM,     TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_ALARMVOICE,    TM_EOD,         ST_DROPCALL,	dropcall	},
	{ ST_ALARMVOICE,    TDX_DIAL,		ST_DROPCALL,	dropcall	},
	{ ST_ALARMVOICE,    TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	

	{ ST_CONNECT,       GCEV_CONNECTED, ST_BRO1,		play	    },
	{ ST_BRO1,          TM_EOD,         ST_LANGUAGE,	play		},
	{ ST_BRO1,          TM_MAXDTMF,     ST_LANGUAGE,	play		},
	{ ST_BRO1,          TM_USRSTOP,     ST_DROPCALL,	dropcall    },	
	{ ST_BRO2,			TM_EOD,         ST_LANGUAGE,	play		},
	{ ST_BRO2,          TM_MAXDTMF,     ST_LANGUAGE,	play		},
	{ ST_BRO2,			TM_USRSTOP,     ST_DROPCALL,	dropcall    },

	{ ST_SYSNOUSE,      TM_EOD,         ST_DROPCALL,	dropcall    },
	{ ST_SYSNOUSE,      TM_MAXDTMF,     ST_DROPCALL,	dropcall    },
	{ ST_SYSNOUSE,      TM_USRSTOP,     ST_DROPCALL,	dropcall    },
	{ ST_NULL,          GCEV_OFFERED,   ST_OFFERED,		play		}
};



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR LpCmdLine,int nCmdShow)
{
	MSG msg;
	
	if (!hPrevInst){
		if(!InitApplication(hInst))
		return(FALSE);
	}

	while (GetMessage(&msg,NULL,0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return nCmdShow;
}

BOOL InitApplication(HINSTANCE hInst)
{

	WNDCLASS wc;
	int i;
	char szChanName[20];
	int row, column;
	char cBuffer[27], tmp[51];
	unsigned long lengthofbuffer;
	int lengthofusername = 9;
	int lengthofpcname = 15;
	FILE *fp;
	int nRetval;

	hInstance = hInst;

	if( cmd_open_database() == FALSE ){
		sys_exit( "Can not connect to database." );
		return FALSE;
	}

	fp = fopen("c:\\windows\\acd_cti.ini","r");
	if ( fp == NULL ){
		sys_exit( "Error open acd_cti.ini" );
		return FALSE;
	}
	nRetval = GetPrivateProfileString("Station","PC_StationID","",tmp,10,"c:\\windows\\acd_cti.ini");
	if( nRetval == 0 ){
		sys_exit( "Error open acd_cti.ini" );
		return FALSE;
	}
	PC_ID[0] = tmp[0];
	PC_ID[1] = tmp[1];
	PC_ID[2] = '\0';

	nRetval = GetPrivateProfileString("Station","Voice_Folder","",tmp,50,"c:\\windows\\acd_cti.ini");
	if( nRetval == 0 ){
		sys_exit( "Error open acd_cti.ini" );
		return FALSE;
	}
	strcpy( VoiceFolder, tmp );
	fclose(fp);

	strcpy( APP_CODE, "01" );
	strcat( APP_CODE, PC_ID );
	
	lengthofbuffer = 25;	
	if( GetUserName(cBuffer,&lengthofbuffer) == 0 )
		strcpy( cBuffer, "Unknown" );
	else
		cBuffer[lengthofusername] = '\0';
	strcpy( m_cUsername, cBuffer );
	
	lengthofbuffer = 25;
	if( GetComputerName(cBuffer,&lengthofbuffer) == 0 )
		strcpy( cBuffer, "Unknown" );
	else
		cBuffer[lengthofpcname] = '\0';
	strcpy( m_cPcname, cBuffer );

	if( cmd_get_parameter() == FALSE ){
		sys_exit( "Fail to get parameter." );
		return FALSE;
	}

	m_outgoing_chans = 0;
	update_audit_trace( IN_DTTM );
	update_station_register( IN_DTTM );

	ZeroMemory(&wc,sizeof(wc));

	strcpy(szChanMsg[0], " incoming call  ");
	for (i=1; i<=MAX_CHANS_PC; i++){
		strcpy(szChanMsg[i],"    Initial     ");
	}

	wc.style         = CS_GLOBALCLASS;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInst; 
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hIcon         = LoadIcon(hInstance,"IDI_APPLICATION");
    wc.hbrBackground = GetStockObject(WHITE_BRUSH); //LTGRAY
    wc.lpszClassName = "MainWndClass"; 

	if( !RegisterClass(&wc) )
      return FALSE;

	wc.lpfnWndProc = ChildProc;
  	wc.hbrBackground = GetStockObject(WHITE_BRUSH); 
   	wc.lpszMenuName = NULL;
   	wc.lpszClassName = "ChildClass"; 
   
   	if( !RegisterClass(&wc) )
		return FALSE;
  
	hWnd = CreateWindow("MainWndClass", " SPF BIVRS System", 
		WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT, 0, MAIN_WND_WIDTH, MAIN_WND_HIGHT, NULL, 
		NULL, hInstance, NULL);
	ShowWindow(hWnd, SW_SHOWNORMAL);

	for (i=1; i<=MAX_CHANS_PC; i++){
		sprintf(szChanName,"    Channel %d",i);
		row = (i - 1)/6 + 1 ;
		column = i%6 ;
	    hChildWnd[i] = CreateWindow("ChildClass",NULL, 
			    WS_CHILD|WS_VISIBLE|WS_BORDER |WS_CLIPSIBLINGS ,
			    getxcord(column), getycord(row),CHILD_WND_WIDTH,
			    CHILD_WND_HIGHT,hWnd, NULL, 
			    hInstance, NULL);
		ShowWindow(hChildWnd[i], SW_SHOWNORMAL);
	
		hStaticText[i] = CreateWindow("STATIC",szChanName,WS_CHILD|WS_VISIBLE|SS_CENTER,0,0,125,20,hChildWnd[i],NULL,hInstance,NULL);
		ShowWindow(hStaticText[i],SW_SHOW);
	}

	hThread = CreateThread(NULL, 0, ThreadDialogic,NULL,CREATE_SUSPENDED, &dThreadid);
	SetThreadPriority ( hThread, THREAD_PRIORITY_NORMAL );
	ResumeThread ( hThread );
	
	return TRUE;
}

int getxcord(int i)
{
	if( i == 0 )
		return(5 + (CHILD_WND_WIDTH + 5) * 5);
	else
		return(5 + (CHILD_WND_WIDTH + 5)* (i-1));
}


int getycord(int i)
{
	return(5 + (CHILD_WND_HIGHT + 5) * (i - 1));
}

int getindexvoice(HWND hwnd)
{
	int i = 1;
	
	while( i <= MAX_CHANS_PC ){
		if( hwnd == hChildWnd[i] )
			break;
		else
			i++;
	}
	if( i > MAX_CHANS_PC )
		i = MAX_CHANS_PC;

	return i;

}

LRESULT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	int i;

	switch(uMsg){      
    	case WM_CREATE:
		break;
	    case WM_CLOSE:
			MessageBox(hWnd, "Exiting Application", "Program Message", MB_ICONSTOP);
			
			update_station_register( OUT_DTTM );
			update_audit_trace( OUT_DTTM );
			for( i = 1; i <= m_total_chans; i ++ ){
				dx_close( oPort[i].voiceh );
			}

			for( i = 1; i <= m_total_chans; i ++ ){
				gc_ResetLineDev( oPort[i].ldev, EV_SYNC );
				gc_Close( oPort[i].ldev );
			}

			cmd_close_database();
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(5);
		default:
			return(DefWindowProc(hwnd,uMsg,wParam,lParam));
	}
	return(DefWindowProc(hwnd,uMsg,wParam,lParam));
}


LRESULT WINAPI ChildProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	HDC hDC;
	PAINTSTRUCT ps;
	int i;

	switch(uMsg){
		case WM_CREATE:
			break;
	    case WM_COMMAND:
			break;
		case WM_PAINT:
			i = getindexvoice( hwnd );
			hDC = BeginPaint( hwnd, &ps );
			TextOut( hDC, 10, 25, szChanMsg[i], strlen(szChanMsg[i]) );
			EndPaint( hwnd, &ps );
			break;
		case WM_USER:
			i = getindexvoice( hwnd );
			hDC = GetDC( hwnd );
			TextOut( hDC, 10, 25, szChanMsg[i], strlen(szChanMsg[i]) );
			ReleaseDC( hwnd, hDC );
			break;
		default:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}
	return( DefWindowProc( hwnd, uMsg, wParam, lParam) );
}

void sys_exit(char * szChannelMsg)
{
    MessageBox(hWnd, szChannelMsg, "Program Message        ", MB_OK);
    PostMessage(hWnd,WM_CLOSE,0,0L);
}

DWORD WINAPI ThreadDialogic(void *x)
{
	
	sys_init();

	wait_event();

	return(0);
}

void sys_init(void)
{
	int i, j;
	int ts, brd;
	char devname[32];
	int mode;
	int brd_offset = 8, first_brd = 1;
	int first_dti_brd = 1;
	int num = 0, ch = 0;
	int nBoard, nChannel;

	memset( &gc_mkcall_blk, 0, sizeof(GC_MAKECALL_BLK) );
	memset( &mkcall_blk, 0, sizeof(MAKECALL_BLK) );

    mkcall_blk.isdn.BC_xfer_cap = BEAR_CAP_SPEECH;
    mkcall_blk.isdn.BC_xfer_mode = ISDN_ITM_CIRCUIT;
    mkcall_blk.isdn.BC_xfer_rate = BEAR_RATE_64KBPS;
    mkcall_blk.isdn.facility_coding_value = ISDN_MEGACOM;
    mkcall_blk.isdn.destination_number_type = NAT_NUMBER; 
    mkcall_blk.isdn.destination_number_plan = ISDN_NUMB_PLAN;
    mkcall_blk.isdn.origination_number_type = NAT_NUMBER;
    mkcall_blk.isdn.origination_number_plan = ISDN_NUMB_PLAN;
    mkcall_blk.isdn.facility_feature_service = ISDN_SERVICE;
    mkcall_blk.isdn.usrinfo_bufp = NULL;
    mkcall_blk.isdn.destination_sub_number_type = 0x02;
    mkcall_blk.isdn.destination_sub_phone_number[0] = '\0';
    gc_mkcall_blk.cclib = &mkcall_blk;

	memset( oPort, 0, sizeof(oPort) );

    mode = SR_STASYNC;
    if( sr_setparm( SRL_DEVICE, SR_MODELTYPE, &mode ) == -1 ){
       sys_exit("sysinit - fail to set srl in async mode");
    }

	mode = SR_POLLMODE;
	if( sr_setparm( SRL_DEVICE, SR_MODEID, &mode ) == -1 ){
		sys_exit("sysinit - fail to set polled mode");
	}

	if( gc_Start(NULL) == -1 ){
		sys_exit("sysinit - fail to start global call");
	}
	
	first_brd = (first_dti_brd - 1) * brd_offset + 1;

	for( i=1, j=first_brd, brd=first_dti_brd; brd <= g_isdnperpc; brd++, j+=brd_offset){
		nBoard = brd;
		for( ts = 1; (ts <= MAX_CHANS_ISDN) && (i <= m_total_chans); ts++, i++ ){
			nChannel = ts;
			sprintf( devname, ":N_%s%dT%d:P_isdn", "dtiB", brd, ts );
			if( gc_Open(&oPort[i].ldev, devname, 0) != GC_SUCCESS  ){
				sys_exit("sysinit - fail to gc_open()");
			}

			num = (ts -1)/4 + j;
			ch = (ts % 4 != 0)? (ts % 4 ) : 4;
			sprintf(devname, "dxxxB%dC%d", num, ch);
			sr_hold();
			if( (oPort[i].voiceh = dx_open(devname,0)) == -1 ){
				sys_exit("sysinit - fail to dx_open()");
				exit(0);
			}
			sr_release();
			if( gc_GetNetworkH(oPort[i].ldev,&oPort[i].timesloth) == -1 ){
				sys_exit("sysinit - fail to gc_GetNetworkH()");
				exit(0);
			}
			oPort[i].nBoard = nBoard;
			oPort[i].nChannel = nChannel;
		}
	}

	for( i = 1; i <= m_total_chans; i++ ){
		if( gc_ResetLineDev(oPort[i].ldev, EV_ASYNC) != GC_SUCCESS ){
			sys_exit("sysinit - fail to gc_resetlinedev()");
		}
		oPort[i].state = ST_NULL;
		oPort[i].lststate = ST_NULL;

		if( gc_SetUsrAttr(oPort[i].ldev, (void *)&oPort[i]) !=GC_SUCCESS ){
			sys_exit("sysint - fail to gc_setusrattr()");
		}

	    if( nr_scroute(oPort[i].timesloth, SC_DTI, oPort[i].voiceh, SC_VOX,	
			SC_FULLDUP) == -1){
			sys_exit("sysint - fail to nr_scroute()");
		}
		
		oPort[i].blocked  = NO;
		oPort[i].in_process = 0;
		oPort[i].switched = INITIAL;
		oPort[i].flag = 0;
	}

	/* Open All English Voice Files */
    if ( ( language_fh = dx_fileopen( "LANGUAGE.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open language.vox");
	}
	if ( ( wronglanguage_fh = dx_fileopen( "WRONGLANGUAGE.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open wronglanguage.vox");
	}
	if ( ( nolanguage_fh = dx_fileopen( "NOLANGUAGE.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open nolanguage.vox");
	}

	if ( ( welcome_efh = dx_fileopen( "WELCOMEe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open WELCOMEe.vox");
	}
    if ( ( sysnouse_efh = dx_fileopen( "SYSNOUSEe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open sysnousee.vox");
	}

    if ( ( empty_efh = dx_fileopen( "EMPTYe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open EMPTYe.vox");
	}

    if ( ( noentry_efh = dx_fileopen( "NOENTRYe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open noentrye.vox");
	}
    if ( ( maxretry_efh = dx_fileopen( "MAXRETRYe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open maxretrye.vox");
	}
    if ( ( wrongservice_efh = dx_fileopen( "WRONGSERVICEe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open wrongservicee.vox");
	}
	if ( ( wrongic_efh = dx_fileopen( "WRONGICe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open wrongice.vox");
	}
	if ( ( wrongnric_efh = dx_fileopen( "WRONGNRICe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open wrongnrice.vox");
	}
	if ( ( wrongfin_efh = dx_fileopen( "WRONGFINe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open wrongfine.vox");
	}
    if ( ( wrongpassport_efh = dx_fileopen( "WRONGPASSPORTe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open wrongpassporte.vox");
	}
	if ( ( wrongdigit_efh = dx_fileopen( "WRONGDIGITe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open wrongdigite.vox");
	}
    if ( ( n0_efh = dx_fileopen( "0e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 0e.vox");
	}
    if ( ( n1_efh = dx_fileopen( "1e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 1e.vox");
	}
    if ( ( n2_efh = dx_fileopen( "2e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 2e.vox");
	}
    if ( ( n3_efh = dx_fileopen( "3e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 3e.vox");
	}
    if ( ( n4_efh = dx_fileopen( "4e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 4e.vox");
	}
    if ( ( n5_efh = dx_fileopen( "5e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 5e.vox");
	}
    if ( ( n6_efh = dx_fileopen( "6e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 6e.vox");
	}
    if ( ( n7_efh = dx_fileopen( "7e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 7e.vox");
	}
    if ( ( n8_efh = dx_fileopen( "8e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 8e.vox");
	}
    if ( ( n9_efh = dx_fileopen( "9e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 9e.vox");
	}
	if ( ( n10_efh = dx_fileopen( "10e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 10e.vox");
	}
    if ( ( n11_efh = dx_fileopen( "11e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 11e.vox");
	}
    if ( ( n12_efh = dx_fileopen( "12e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 12e.vox");
	}
    if ( ( n13_efh = dx_fileopen( "13e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 13e.vox");
	}
    if ( ( n14_efh = dx_fileopen( "14e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 14e.vox");
	}
    if ( ( n15_efh = dx_fileopen( "15e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 15e.vox");
	}
    if ( ( n16_efh = dx_fileopen( "16e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 16e.vox");
	}
    if ( ( n17_efh = dx_fileopen( "17e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 17e.vox");
	}
    if ( ( n18_efh = dx_fileopen( "18e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 18e.vox");
	}
    if ( ( n19_efh = dx_fileopen( "19e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 19e.vox");
	}
	if ( ( n20_efh = dx_fileopen( "20e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 20e.vox");
	}
	if ( ( n30_efh = dx_fileopen( "30e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 30e.vox");
	}
    if ( ( n40_efh = dx_fileopen( "40e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 40e.vox");
	}
    if ( ( n50_efh = dx_fileopen( "50e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 50e.vox");
	}
    if ( ( n60_efh = dx_fileopen( "60e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 60e.vox");
	}
    if ( ( n70_efh = dx_fileopen( "70e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 70e.vox");
	}
    if ( ( n80_efh = dx_fileopen( "80e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 80e.vox");
	}
    if ( ( n90_efh = dx_fileopen( "90e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 90e.vox");
	}
    if ( ( n100_efh = dx_fileopen( "100e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 100e.vox");
	}
    if ( ( n1000_efh = dx_fileopen( "1000e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 1000e.vox");
	}
	if ( ( cent_efh = dx_fileopen( "CENTe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open cente.vox");
	}
	if ( ( cents_efh = dx_fileopen( "CENTSe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open centse.vox");
	}
    if ( ( dollar_efh = dx_fileopen( "DOLLARe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open dollare.vox");
	}
	if ( ( dollars_efh = dx_fileopen( "DOLLARSe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open dollarse.vox");
	}
	if ( ( and_efh = dx_fileopen( "ANDe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open ande.vox");
	}
    if ( ( service_efh = dx_fileopen( "SERVICEe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open servicee.vox");
	}
	if ( ( ic_efh = dx_fileopen( "ICe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open ice.vox");
	}
    if ( ( nric_efh = dx_fileopen( "NRICe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open nrice.vox");
	}
	if ( ( fin_efh = dx_fileopen( "FINe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open fine.vox");
	}
    if ( ( passport_efh = dx_fileopen( "PASSPORTe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open passporte.vox");
	}
    if ( ( repeatnric_efh = dx_fileopen( "REPEATNRICe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open repeatnrice.vox");
	}
    if ( ( confirm_efh = dx_fileopen( "CONFIRMe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open confirme.vox");
	}
    
	if ( ( notmob_efh = dx_fileopen( "NOTMOBe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open notmobe.vox");
	}
    if ( ( endcall_efh = dx_fileopen( "FINISHe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open endcalle.vox");
	}
    if ( ( acknowledged_efh = dx_fileopen( "REGISTEREDe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open registerede.vox");
	}
    if ( ( deleted_efh = dx_fileopen( "DELETEDe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open deletede.vox");
	}
	if ( ( repeat_efh = dx_fileopen( "REPEATe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open repeate.vox");
	}
	if ( ( alarm_efh = dx_fileopen( "ALARMe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open alarme.vox");
	}
	
	
	/* Open All Chinese Voice Files */
    if ( ( welcome_cfh = dx_fileopen( "WELCOMEc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open WELCOMEc.vox");
	}
    if ( ( sysnouse_cfh = dx_fileopen( "SYSNOUSEc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open sysnousec.vox");
	}

    if ( ( empty_cfh = dx_fileopen( "EMPTYc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open EMPTYc.vox");
	}

    if ( ( noentry_cfh = dx_fileopen( "NOENTRYc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open noentryc.vox");
	}
    if ( ( maxretry_cfh = dx_fileopen( "MAXRETRYc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open maxretryc.vox");
	}
    if ( ( wrongservice_cfh = dx_fileopen( "WRONGSERVICEc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open wrongservicec.vox");
	}
	if ( ( wrongic_cfh = dx_fileopen( "WRONGICc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open wrongicc.vox");
	}
	if ( ( wrongnric_cfh = dx_fileopen( "WRONGNRICc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open wrongnricc.vox");
	}
	if ( ( wrongfin_cfh = dx_fileopen( "WRONGFINc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open wrongfinc.vox");
	}
    if ( ( wrongpassport_cfh = dx_fileopen( "WRONGPASSPORTc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open wrongpassportc.vox");
	}
	if ( ( wrongdigit_cfh = dx_fileopen( "WRONGDIGITc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open wrongdigitc.vox");
	}
    if ( ( n0_cfh = dx_fileopen( "0c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 0c.vox");
	}
    if ( ( n1_cfh = dx_fileopen( "1c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 1c.vox");
	}
    if ( ( n2_cfh = dx_fileopen( "2c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 2c.vox");
	}
    if ( ( n3_cfh = dx_fileopen( "3c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 3c.vox");
	}
    if ( ( n4_cfh = dx_fileopen( "4c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 4c.vox");
	}
    if ( ( n5_cfh = dx_fileopen( "5c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 5c.vox");
	}
    if ( ( n6_cfh = dx_fileopen( "6c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 6c.vox");
	}
    if ( ( n7_cfh = dx_fileopen( "7c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 7c.vox");
	}
    if ( ( n8_cfh = dx_fileopen( "8c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 8c.vox");
	}
    if ( ( n9_cfh = dx_fileopen( "9c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 9c.vox");
	}
	if ( ( n10_cfh = dx_fileopen( "10c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 10c.vox");
	}
    if ( ( n11_cfh = dx_fileopen( "11c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 11c.vox");
	}
    if ( ( n12_cfh = dx_fileopen( "12c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 12c.vox");
	}
    if ( ( n13_cfh = dx_fileopen( "13c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 13c.vox");
	}
    if ( ( n14_cfh = dx_fileopen( "14c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 14c.vox");
	}
    if ( ( n15_cfh = dx_fileopen( "15c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 15c.vox");
	}
    if ( ( n16_cfh = dx_fileopen( "16c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 16c.vox");
	}
    if ( ( n17_cfh = dx_fileopen( "17c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 17c.vox");
	}
    if ( ( n18_cfh = dx_fileopen( "18c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 18c.vox");
	}
    if ( ( n19_cfh = dx_fileopen( "19c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 19c.vox");
	}
	if ( ( n20_cfh = dx_fileopen( "20c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 20c.vox");
	}
	if ( ( n30_cfh = dx_fileopen( "30c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 30c.vox");
	}
    if ( ( n40_cfh = dx_fileopen( "40c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 40c.vox");
	}
    if ( ( n50_cfh = dx_fileopen( "50c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 50c.vox");
	}
    if ( ( n60_cfh = dx_fileopen( "60c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 60c.vox");
	}
    if ( ( n70_cfh = dx_fileopen( "70c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 70c.vox");
	}
    if ( ( n80_cfh = dx_fileopen( "80c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 80c.vox");
	}
    if ( ( n90_cfh = dx_fileopen( "90c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 90c.vox");
	}
    if ( ( n100_cfh = dx_fileopen( "100c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 100c.vox");
	}
    if ( ( n1000_cfh = dx_fileopen( "1000c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open 1000c.vox");
	}
	if ( ( cent_cfh = dx_fileopen( "CENTc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open centc.vox");
	}
	if ( ( cents_cfh = dx_fileopen( "CENTSc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open centsc.vox");
	}
    if ( ( dollar_cfh = dx_fileopen( "DOLLARc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open dollarc.vox");
	}
	if ( ( dollars_cfh = dx_fileopen( "DOLLARSc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open dollarsc.vox");
	}
	if ( ( and_cfh = dx_fileopen( "ANDc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open andc.vox");
	}
    if ( ( service_cfh = dx_fileopen( "SERVICEc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open servicec.vox");
	}
	if ( ( ic_cfh = dx_fileopen( "ICc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open icc.vox");
	}
    if ( ( nric_cfh = dx_fileopen( "NRICc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open nricc.vox");
	}
	if ( ( fin_cfh = dx_fileopen( "FINc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open finc.vox");
	}
    if ( ( passport_cfh = dx_fileopen( "PASSPORTc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open passportc.vox");
	}
    if ( ( repeatnric_cfh = dx_fileopen( "REPEATNRICc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open repeatnricc.vox");
	}
    if ( ( confirm_cfh = dx_fileopen( "CONFIRMc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open confirmc.vox");
	}
    
	if ( ( notmob_cfh = dx_fileopen( "NOTMOBc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open notmobc.vox");
	}
    if ( ( endcall_cfh = dx_fileopen( "FINISHc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open endcallc.vox");
	}
    if ( ( acknowledged_cfh = dx_fileopen( "REGISTEREDc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open registeredc.vox");
	}
    if ( ( deleted_cfh = dx_fileopen( "DELETEDc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open deletedc.vox");
	}
	if ( ( repeat_cfh = dx_fileopen( "REPEATc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open repeatc.vox");
	}
	if ( ( alarm_cfh = dx_fileopen( "ALARMc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open alarmc.vox");
	}
	if ( ( broadcast1_efh = dx_fileopen( "broadcast1e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open broadcast1e.vox");
	}
	if ( ( broadcast1_cfh = dx_fileopen( "broadcast1c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open broadcast1c.vox");
	}
	if ( ( broadcast2_efh = dx_fileopen( "broadcast2e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open broadcast2e.vox");
	}
	if ( ( broadcast2_cfh = dx_fileopen( "broadcast2c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open broadcast2c.vox");
	}
	if ( ( ack1_efh = dx_fileopen( "ACKNOWLEDGE1e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open ACKNOWLEDGE1e.VOX");
	}
	if ( ( ack1_cfh = dx_fileopen( "ACKNOWLEDGE1c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open ACKNOWLEDGE1c.VOX");
	}
	if ( ( ack2_efh = dx_fileopen( "ACKNOWLEDGE2e.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open ACKNOWLEDGE2e.VOX");
	}
	if ( ( ack2_cfh = dx_fileopen( "ACKNOWLEDGE2c.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open ACKNOWLEDGE2c.VOX");
	}

	// 2011-02-08 ack sms
	if ( ( acksms_efh = dx_fileopen( "ACKSMSe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open ACKSMSe.VOX");
	}
	if ( ( acksms_cfh = dx_fileopen( "ACKSMSc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open ACKSMSc.VOX");
	}
	if ( ( mobileno_efh = dx_fileopen( "Mobilenoe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open Mobilenoe.VOX");
	}
	if ( ( mobileno_cfh = dx_fileopen( "Mobilenoc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open Mobilenoc.VOX");
	}
	if ( ( wrongmobileno_efh = dx_fileopen( "WrongMobilenoe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open WrongMobilenoe.VOX");
	}
	if ( ( wrongmobileno_cfh = dx_fileopen( "WrongMobilenoc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open WrongMobilenoc.VOX");
	}
	if ( ( recvsms_efh = dx_fileopen( "RecvSMSe.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open RecvSMSe.VOX");
	}
	if ( ( recvsms_cfh = dx_fileopen( "RecvSMSc.VOX", O_RDONLY|O_BINARY ) ) == -1 ) {
       sys_exit("fail to open RecvSMSc.VOX");
	}
	
}

void wait_event()
{
	int nRetval;
	char cStatement[270];
	long lTimeleft = 0;
	long lTimecount0 = 0;
	long lTimecount1 = 0;
	int nMob = FALSE;
	int i, n;

	CRN crn;
	struct linebag *pPhoneline;
	int code;
	int devh;
	int devtype;
	void *evtdatap;
	METAEVENT metaevent;
	int bitmask;
	void *pPoint;
    char cCallerid[50];
	char cDNIS[50];
	int channel;
		
	for( ; ; ){
		lTimeleft = sr_waitevt( 50 );
		if( lTimeleft < 0 || lTimeleft >= 50 ){
			lTimecount0 = lTimecount0 + 50;
			lTimecount1 = lTimecount1 + 50;
		}
		else{
			lTimecount0 = lTimecount0 + 50 - lTimeleft;
			lTimecount1 = lTimecount1 + 50 - lTimeleft;
		}
		if( lTimecount0 > 15000 ){
			lTimecount0 = 0;
			if( hdbc == NULL ){
				cmd_open_database();
			}
			else{
				cmd_get_parameter();
				update_station_register( UPDATE_DTTM );
//				check_alarm_log();
				nMob = check_broadcast_timekeeper();
			}
		}
		if( nMob == TRUE ){
			if( lTimeleft < 0 || lTimecount1 > 1000 ){
				lTimecount1 = 0;
				check_mobnominalroll();
			}
		}
		else{
			if( lTimecount1 > 7000 ){
				lTimecount1 = 0;
				check_mobnominalroll();
			}
		}

		if( lTimeleft < 0 )
			continue;

		devh = (int)sr_getevtdev(0);
		code = (int)sr_getevttype(0);
		evtdatap = (void *) sr_getevtdatap(0);
		n = (int) sr_getevtlen(0);

		if( devh % 2 == 0 )
			channel = devh / 2;
		else
			channel = ( devh - 1 ) / 2;

		switch( code ){
		case 2081:
			sprintf( cStatement, "GCEV_ALERTING %d", code );
			break;
		case 2082:
			sprintf( cStatement, "GCEV_CONNECTED (Outgoing) %d", code );
			sys_update_log( channel, LOG_EVT, cStatement );
			break;
		case 2084:
			sprintf( cStatement, "GCEV_OFFERED %d", code );
			break;
		case 2086:
			sprintf( cStatement, "GCEV_DISCONNECTED (Remote end) %d", code );
			sys_update_log( channel, LOG_EVT, cStatement );
			break;
		case 2087:
			sprintf( cStatement, "GCEV_PROCEEDING %d", code );
			break;
		case 2088:
			sprintf( cStatement, "GCEV_PROGRESSING %d", code );
			break;
		case 2089:
			sprintf( cStatement, "GCEV_USRINFO %d", code );
			break;

		case 2050:
			sprintf( cStatement, "GCEV_ANSWERED (Incoming) %d", code );
			sys_update_log( channel, LOG_EVT, cStatement );
			break;
		case 2051:
			sprintf( cStatement, "GCEV_CALLPROGRESS %d", code );
			break;
		case 2052:
			sprintf( cStatement, "GCEV_ACCEPT %d", code );
			break;
		case 2053:
			sprintf( cStatement, "GCEV_DROPCALL %d", code );
			sys_update_log( channel, LOG_EVT, cStatement );
			break;
		case 2054:
			sprintf( cStatement, "GCEV_RESETLINEDEV %d", code );
			break;
		
		case 129:
			sprintf( cStatement, "Play completed %d", code );
			//sys_update_log( channel, LOG_EVT, cStatement );
			break;
		case 131:
			sprintf( cStatement, "Get Digits Completed %d", code );
			//sys_update_log( channel, LOG_EVT, cStatement );
			break;
		case 132:
			sprintf( cStatement, "Dial Completed %d", code );
			sys_update_log( channel, LOG_EVT, cStatement );
			break;
		default:
			sprintf( cStatement, "sr_getevttype() = %d", code );
			sys_update_log( channel, LOG_EVT, cStatement );
			break;
		}
		
		if( (code & DT_GC) == DT_GC ){

			/* Retrieve the Metaevent block */
			nRetval = gc_GetMetaEvent( &metaevent );
			if ( nRetval != EGC_NOERR) {
                sprintf( cStatement, "gc_GetMetaEvent( &metaevent ) return %d", nRetval );
				sys_update_log( channel, LOG_ERR, cStatement );
				continue;
			}

			code = (int) metaevent.evttype;
			devh = (int) metaevent.evtdev;
			evtdatap = (void *) metaevent.evtdatap;
			n = (int) metaevent.evtlen;
			
			nRetval = gc_GetUsrAttr( metaevent.linedev, &pPoint );
			if( nRetval != EGC_NOERR ){
				sprintf( cStatement, "gc_GetUsrAttr( %d, &pPoint ) return %d", metaevent.linedev, nRetval );
				sys_update_log( channel, LOG_ERR, cStatement );
				continue;
			}
			pPhoneline = (struct linebag *)pPoint;

			nRetval = gc_GetCRN( &crn,  &metaevent );
			if( nRetval != EGC_NOERR ){
                sprintf( cStatement, "gc_GetCRN( &crn,  &metaevent ) return %d", nRetval );
				sys_update_log( channel, LOG_ERR, cStatement );
			} 
			else if( crn ){
				pPhoneline->crn = crn;
			}
				
			devtype = GCAPI;
			if( pPhoneline->state == ST_RESET ){
				
				if( code != GCEV_RESETLINEDEV ){
					
					if( code == GCEV_BLOCKED )
						pPhoneline->blocked = YES;
					if( code == GCEV_UNBLOCKED )
						pPhoneline->blocked = NO;
					
					continue;
				}
			}

			if( devh > 2 * MAX_CHANS_ISDN + 1 ){
				devh = devh - 1;
			}

			switch( code ){

			case GCEV_RESETLINEDEV:
				nRetval = gc_WaitCall( pPhoneline->ldev, NULL, NULL, 0, EV_ASYNC );
				if( nRetval != EGC_NOERR ){
					sprintf( cStatement, "gc_WaitCall( pPhoneline->ldev, NULL, NULL, 0, EV_ASYNC ) return %d", nRetval );
					sys_update_log( channel, LOG_ERR, cStatement );
				}

				pPhoneline->state = ST_NULL;
				pPhoneline->lststate = ST_NULL;
				pPhoneline->flag = 0;
				pPhoneline->switched = INITIAL;
				pPhoneline->in_process = 0;
				
				pPhoneline->nMobid = 0;
				pPhoneline->nWhichcontactno = 0;
				pPhoneline->nWhichbroadcast = 0;
				strcpy(pPhoneline->cExcuseflg, "N" );
				strcpy(pPhoneline->cRegistercode, "" ); 
				strcpy(pPhoneline->cOutstanding, "" ); 
				pPhoneline->nPriority = 0;
				strcpy(pPhoneline->cName, "");
				strcpy(pPhoneline->cNRIC, "");
				strcpy(pPhoneline->cPin, "");
				strcpy(pPhoneline->cAckMobileno, "");
				strcpy(pPhoneline->cContactno, "");
				pPhoneline->cCalltype = ' ';
				strcpy(pPhoneline->cBrofile1, "");
				strcpy(pPhoneline->cBrofile2, "");
				strcpy(pPhoneline->cAckfile1, "");
				strcpy(pPhoneline->cAckfile2, "");
				strcpy(pPhoneline->cCalldigits, "" );
				pPhoneline->cAckstatus = 'N';
				pPhoneline->nCallresult = 0;
				pPhoneline->cBroreset = 'N';
				pPhoneline->noofretry = 0;
				pPhoneline->noofpin = 0;
				pPhoneline->noofreplay = 0;
				pPhoneline->noofmobileno = 0;
				strcpy(pPhoneline->playnumber, "" );
				pPhoneline->playno = 0;
				strcpy(pPhoneline->cAlarmcode, "" );
				pPhoneline->nAlarmid = 0;

				pPhoneline->nLanguage = 0;
				pPhoneline->nICtype = 0;
				pPhoneline->nAcksms = 0;
				pPhoneline->nAdhocid = 0;

				wsprintf(szChanMsg[devh/2],"    Waiting     "); 
				PostMessage(hChildWnd[devh/2],WM_USER,0,20);
				break;

			case GCEV_DISCONNECTED:
				
				if( pPhoneline->in_process == 0 ){
					nRetval = gc_DropCall( pPhoneline->crn, GC_NORMAL_CLEARING, EV_ASYNC );
					if( nRetval != EGC_NOERR ){
						sprintf( cStatement, "gc_DropCall( pPhoneline->crn, GC_NORMAL_CLEARING, EV_ASYNC ) return %d", nRetval );
						sys_update_log( channel, LOG_ERR, cStatement );
					}
					if( dx_stopch(pPhoneline->voiceh, EV_ASYNC) == -1 ){
						sprintf( cStatement, "dx_stopch(pPhoneline->voiceh, EV_ASYNC) return %d", -1 );
						sys_update_log( channel, LOG_ERR, cStatement );
					}
				}
				else{
					nRetval = gc_DropCall( pPhoneline->crn, GC_NORMAL_CLEARING, EV_ASYNC );
					if( dx_stopch(pPhoneline->voiceh, EV_ASYNC) == -1 ){
						sprintf( cStatement, "dx_stopch(pPhoneline->voiceh, EV_ASYNC) return %d", -1 );
						sys_update_log( channel, LOG_ERR, cStatement );
					}
				}
				pPhoneline->in_process = 0;
				break;

			case GCEV_DROPCALL:

				time( &pPhoneline->end_dttm );
				if( pPhoneline->state == ST_MACKCALL ){
					pPhoneline->nCallresult = CALL_FAIL; // call fail
					if( pPhoneline->switched == OUTGOING )
						update_mobnominalroll( pPhoneline->nMobid, pPhoneline->nWhichcontactno, pPhoneline->cPin, 'L', 0 );
					else
						update_alarm_log( pPhoneline->nAlarmid, 'F' );
				}

				if( pPhoneline->state == ST_PROCEEDING ){
					pPhoneline->nCallresult = CALL_ENGAGE; // call engage
					if( pPhoneline->switched == OUTGOING )
						update_mobnominalroll( pPhoneline->nMobid, pPhoneline->nWhichcontactno, pPhoneline->cPin, 'E', 0 );
					else
						update_alarm_log( pPhoneline->nAlarmid, 'E' );
				}

				if( pPhoneline->state == ST_CALLRING ){
					pPhoneline->nCallresult = CALL_NOANSWER; // call no answer
					if( pPhoneline->switched == OUTGOING )
						update_mobnominalroll( pPhoneline->nMobid, pPhoneline->nWhichcontactno, pPhoneline->cPin, 'R', 0 );
					else
						update_alarm_log( pPhoneline->nAlarmid, 'R' );
				}
				if( pPhoneline->switched == OUTGOING ){
					m_outgoing_chans = m_outgoing_chans - 1;
				}
				insert_broadcast_detail( devh, GCEV_DROPCALL );	
				
				nRetval = gc_ResetLineDev( pPhoneline->ldev, EV_ASYNC );
				if( nRetval != EGC_NOERR ){
					sprintf( cStatement, "gc_ResetLineDev( pPhoneline->ldev, EV_ASYNC ) return %d", nRetval );
					sys_update_log( channel, LOG_ERR, cStatement );
				}
				pPhoneline->state = ST_RESET;
				break;

			case GCEV_PROCEEDING: 
				wsprintf( szChanMsg[devh/2],"    Outgoing    " ); 
				PostMessage( hChildWnd[devh/2],WM_USER,0,20 );
				pPhoneline->state = ST_PROCEEDING;
				break;

			case GCEV_BLOCKED:
				wsprintf( szChanMsg[devh/2],"     Blocked    " ); 
				PostMessage( hChildWnd[devh/2],WM_USER,0,20 );
				pPhoneline->blocked = YES;
				break;

			case GCEV_UNBLOCKED:
				wsprintf( szChanMsg[devh/2],"    Waiting     "); 
				pPhoneline->blocked = NO;
				break;

			case GCEV_OFFERED: // incoming start
				pPhoneline->flag = 1;
				pPhoneline->switched = INCOMING;

				strcpy(cCallerid, "" );
				time( &pPhoneline->start_dttm );
				pPhoneline->connect_dttm = 0;
				time( &pPhoneline->end_dttm );
				pPhoneline->nMobid = 0;
				pPhoneline->nWhichcontactno = 0;
				pPhoneline->nWhichbroadcast = 0;
				strcpy(pPhoneline->cExcuseflg, "N" );
				strcpy(pPhoneline->cRegistercode, "" );
				strcpy(pPhoneline->cOutstanding, "" );
				pPhoneline->nPriority = 0;
				strcpy(pPhoneline->cName, "");
				strcpy(pPhoneline->cNRIC, "");
				strcpy(pPhoneline->cPin, "");
				strcpy(pPhoneline->cAckMobileno, "");
				strcpy(pPhoneline->cContactno, "");
				pPhoneline->cCalltype = ' ';
				strcpy(pPhoneline->cBrofile1, "");
				strcpy(pPhoneline->cBrofile2, "");
				strcpy(pPhoneline->cAckfile1, "");
				strcpy(pPhoneline->cAckfile2, "");
				strcpy(pPhoneline->cCalldigits, "" );
				pPhoneline->cAckstatus = 'N';
				pPhoneline->nCallresult = 0;
				pPhoneline->cBroreset = 'N';
				pPhoneline->noofretry = 0;
				pPhoneline->noofpin = 0;
				pPhoneline->noofreplay = 0;
				pPhoneline->noofmobileno = 0;
				strcpy(pPhoneline->playnumber, "" );
				pPhoneline->playno = 0;
				strcpy(pPhoneline->cAlarmcode, "" );
				pPhoneline->nAlarmid = 0;
				nRetval = gc_GetANI( pPhoneline->crn, cCallerid );
				cCallerid[8] = '\0';
				strcpy( pPhoneline->cContactno, cCallerid );

				nRetval = gc_GetDNIS( pPhoneline->crn, cDNIS );
				cDNIS[8] = '\0';

				pPhoneline->nLanguage = 0;
				pPhoneline->nICtype = 0;
				pPhoneline->nAcksms = 0;
				pPhoneline->nAdhocid = 0;
				// incoming call
				nRetval = gc_AcceptCall(pPhoneline->crn, 0, EV_ASYNC);
				if( nRetval != EGC_NOERR ){
					sprintf( cStatement, "gc_AcceptCall(pPhoneline->crn, 0, EV_ASYNC) return %d", nRetval );
					sys_update_log( channel, LOG_ERR, cStatement );
				}
				break;

			case GCEV_ACCEPT:
				pPhoneline->cContactno[8] = '\0';
				nRetval = check_ic_type(devh, pPhoneline->cContactno);
//				nMob = check_broadcast_timetable(pPhoneline->cContactno);
//				if( nMob == FALSE ){
//					dropcall( devh );
//				}
//				else{
					nRetval = gc_AnswerCall(pPhoneline->crn, 0, EV_ASYNC);
					if( nRetval != EGC_NOERR) {
						sprintf( cStatement, "gc_AnswerCall(pPhoneline->crn, 0, EV_ASYNC) return %d", nRetval );
						sys_update_log( channel, LOG_ERR, cStatement );
					}
					pPhoneline->state = ST_ANSWERCALL;
//				}
				

				break;

			case GCEV_ANSWERED:
				pPhoneline->in_process = 1;
				time( &pPhoneline->connect_dttm );
				time( &pPhoneline->end_dttm );
				
				wsprintf( szChanMsg[devh/2],"    Incoming    " ); 
				PostMessage( hChildWnd[devh/2],WM_USER,0,20 );
				check_event( devh, code );
				break;

			case GCEV_CONNECTED:
				
				time( &pPhoneline->connect_dttm );
				time( &pPhoneline->end_dttm );
				
				if( pPhoneline->switched == ALARM ){
					pPhoneline->state = ST_ALARM;
					update_alarm_log( pPhoneline->nAlarmid, 'S' );
				}
				else{
					update_mobnominalroll( pPhoneline->nMobid, pPhoneline->nWhichcontactno, pPhoneline->cPin, 'A', 0 );
					pPhoneline->state = ST_CONNECT;
				}
				
				check_event( devh, code );
				break;

			case GCEV_CALLSTATUS:
				break;

			case GCEV_ALERTING:
				pPhoneline -> state = ST_CALLRING;
				break;

			case GCEV_TASKFAIL:
				nRetval = gc_ResetLineDev( pPhoneline->ldev, EV_ASYNC );
				if( nRetval != EGC_NOERR ){
					sprintf( cStatement, "gc_ResetLineDev( pPhoneline->ldev, EV_ASYNC ) return %d", nRetval );
					sys_update_log( channel, LOG_ERR, cStatement );
				}
				pPhoneline->state = ST_RESET;
				break;
			}
		} // (code & DT_GC) == DT_GC
		else if( (code & DT_DTI) == DT_DTI ){
			continue;
		}
		else{
			for( i = 1; i <= m_total_chans; i ++ ){
				if( oPort[i].voiceh == devh )
					break;
			}

			if( i > m_total_chans ){
				break;
			}
			else
				pPhoneline = &oPort[i];

			bitmask = (int) ATDX_TERMMSK(pPhoneline->voiceh);
			devtype = D40;

			if( devh > 2 * MAX_CHANS_ISDN + 1 ){
				devh = devh - 2;
			}
			else{
				devh = devh - 1;
			}

			if( code == TDX_DIAL ){
				check_event( devh, code );
			}

			if( bitmask & TM_LCOFF ){
				sys_update_log( channel, LOG_EVT, "TM_LCOFF" );
                check_event( devh, bitmask );
			}
			
			if( bitmask & TM_MAXDTMF ){
				sys_update_log( channel, LOG_EVT, "TM_MAXDTMF" );
				if( (pPhoneline->state == ST_BRO1) || (pPhoneline->state == ST_BRO2) ){
//					if( dx_fileclose(msgfh[devh/2])== -1 ){
//						sprintf( cStatement, "dx_fileclose(msgfh[%d]) return -1", devh/2 );
//						sys_update_log( channel, LOG_ERR, cStatement );
//					}
				}
				if( (pPhoneline->state == ST_ACK1) || (pPhoneline->state == ST_ACK2) ){
//					if( dx_fileclose(msgfh[devh/2]) == -1 ){
//						sprintf( cStatement, "dx_fileclose(msgfh[%d]) return -1", devh/2 );
//						sys_update_log( channel, LOG_ERR, cStatement );
//					}
				}
                check_event( devh, bitmask );
			}

			if( bitmask & TM_EOD ){
				sys_update_log( channel, LOG_EVT, "TM_EOD" );
				if( (pPhoneline->state == ST_BRO1) || (pPhoneline->state == ST_BRO2) ){
//					if (dx_fileclose(msgfh[devh/2])== -1 ){
//						sprintf( cStatement, "dx_fileclose(msgfh[%d]) return -1", devh/2 );
//						sys_update_log( channel, LOG_ERR, cStatement );
//					}
				}
				if( (pPhoneline->state == ST_ACK1) || (pPhoneline->state == ST_ACK2) ){
//					if( dx_fileclose(msgfh[devh/2]) == -1 ){
//						sprintf( cStatement, "dx_fileclose(msgfh[%d]) return -1", devh/2 );
//						sys_update_log( channel, LOG_ERR, cStatement );
//					}
				}
                check_event(devh,bitmask);
			}

			if( bitmask & TM_MAXTIME ){
                sys_update_log( channel, LOG_EVT, "TM_MAXTIME" );
				check_event( devh, bitmask );
			}
			
			if( bitmask & TM_USRSTOP ){
				sys_update_log( channel, LOG_EVT, "TM_USRSTOP" );
				if( (pPhoneline->state == ST_BRO1) || (pPhoneline->state == ST_BRO2) ){
					//if (dx_fileclose(msgfh[devh/2])== -1 ){
					//	sprintf( cStatement, "dx_fileclose(msgfh[%d]) return -1", devh/2 );
					//	sys_update_log( channel, LOG_ERR, cStatement );
					//}
				}
				if( (pPhoneline->state == ST_ACK1) || (pPhoneline->state == ST_ACK2) ){
					//if( dx_fileclose(msgfh[devh/2]) == -1 ){
					//	sprintf( cStatement, "dx_fileclose(msgfh[%d]) return -1", devh/2 );
					//	sys_update_log( channel, LOG_ERR, cStatement );
					//}
				}
				check_event(devh,bitmask);
			}

			if ( bitmask & TM_DIGIT ){
			   sys_update_log( channel, LOG_EVT, "TM_DIGIT" );
               check_event( devh, bitmask );
			}
		}

	}   
}


void check_event(int devh, int code )
{
	int i;
	void (*func_ptr)(int);
	int devh1 = devh/2;
	
	for( i = 0; i < TABLE_SIZES; i ++ ){
		if( oPort[devh1].state == table[i].current_state && code == table[i].event ){
			func_ptr = table[i].funcptr;
			oPort[devh1].state = table[i].next_state;
			(*func_ptr)( devh );
			break;
		}
	}

	if (i >= TABLE_SIZES){
	}
}

void play(int devh)
{
   int nFiledesc;
   char tmp[127];
   int devh1;
   
   devh1 = devh/2;
   
   /* Seach Play Message */
   switch( oPort[devh1].state ){
   case ST_WELCOME:
	   if( oPort[devh1].nLanguage == 2 )
		   nFiledesc = welcome_cfh;
	   else
		   nFiledesc = welcome_efh;
	   break;

   case ST_LANGUAGE:
	   nFiledesc = language_fh;
	   break;
   
   case ST_NOLANGUAGE:
	   nFiledesc = nolanguage_fh;
	   break;

   case ST_WRONGLANGUAGE:
	   nFiledesc = wronglanguage_fh;
	   break;

   case ST_SERVICE:
	   if( oPort[devh1].nLanguage == 2 )
		   nFiledesc = service_cfh;
	   else
		   nFiledesc = service_efh;
	   break;

   case ST_NRIC:
	   if( oPort[devh1].nLanguage == 2 ){
		   nFiledesc = ic_cfh;
		   if( oPort[devh1].nICtype == 1 )
			   nFiledesc = nric_cfh;
		   if( oPort[devh1].nICtype == 2 )
			   nFiledesc = fin_cfh;
		   if( oPort[devh1].nICtype == 3 )
			   nFiledesc = passport_cfh;
	   }
	   else{
		   nFiledesc = ic_efh;
		   if( oPort[devh1].nICtype == 1 )
			   nFiledesc = nric_efh;
		   if( oPort[devh1].nICtype == 2 )
			   nFiledesc = fin_efh;
		   if( oPort[devh1].nICtype == 3 )
			   nFiledesc = passport_efh;
	   }
	   break;
      
   case ST_CONFIRMDIG:
	   if( oPort[devh1].nLanguage == 2 )
		   nFiledesc = confirm_cfh;
	   else
		   nFiledesc = confirm_efh;
	   break;
   
   case ST_CONFIRMMNO:
	   if( oPort[devh1].nLanguage == 2 )
		   nFiledesc = confirm_cfh;
	   else
		   nFiledesc = confirm_efh;
	   break;

   case ST_ALARMVOICE:
	   if( oPort[devh1].nLanguage == 2 )
		   nFiledesc = alarm_cfh;
	   else
		   nFiledesc = alarm_efh;
	   break;

   case ST_BRO1:
	   if( oPort[devh1].nLanguage == 2 ){
		   nFiledesc = broadcast1_cfh;
		   //msgfh[devh1] = dx_fileopen( "broadcast1c.VOX", O_RDONLY|O_BINARY );
	   }
	   else{
			nFiledesc = broadcast1_efh;
		   //msgfh[devh1] = dx_fileopen( "broadcast1e.VOX", O_RDONLY|O_BINARY );
	   }
	   //nFiledesc = msgfh[devh1];
	   break;
   
   case ST_BRO2:
	   if( oPort[devh1].nLanguage == 2 ){
		   nFiledesc = broadcast2_cfh;
		   //msgfh[devh1] = dx_fileopen( "broadcast2c.VOX", O_RDONLY|O_BINARY );
	   }
	   else{
		   nFiledesc = broadcast2_efh;
		   //msgfh[devh1] = dx_fileopen( "broadcast2e.VOX", O_RDONLY|O_BINARY );
	   }
	   //nFiledesc = msgfh[devh1];
	   break;
   
   case ST_MOBILENO:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = mobileno_cfh;
		else
			nFiledesc = mobileno_efh;
		break;

   case ST_ACKSMS:
	   if( oPort[devh1].nLanguage == 2 )
		   nFiledesc = acksms_cfh;
	   else
		   nFiledesc = acksms_efh;
	   break;
   
   case ST_REPEATACK:
		if( oPort[devh1].nAcksms > 0 ){ // Mobile number in the list
			if( oPort[devh1].nLanguage == 2 ){
				if( oPort[devh1].noofreplay > g_ivrnoretry - 1 ){
					oPort[devh1].state = ST_ENDCALL;
					nFiledesc = endcall_cfh;
				}
				else{
					oPort[devh1].noofreplay = oPort[devh1].noofreplay + 1;
					nFiledesc = repeat_cfh;
				}
			}
			else{
				if( oPort[devh1].noofreplay > g_ivrnoretry - 1 ){
					oPort[devh1].state = ST_ENDCALL;
					nFiledesc = endcall_efh;
				}
				else{
					oPort[devh1].noofreplay = oPort[devh1].noofreplay + 1;
					nFiledesc = repeat_efh;
				}
			}
		}
		else{
			oPort[devh1].state = ST_ACKSMS;
			if( oPort[devh1].nLanguage == 2 ){
				if( oPort[devh1].noofreplay > g_ivrnoretry - 1 ){
					oPort[devh1].state = ST_ENDCALL;
					nFiledesc = endcall_cfh;
				}
				else{
					oPort[devh1].noofreplay = oPort[devh1].noofreplay + 1;
					nFiledesc = acksms_cfh;
				}
			}
			else{
				if( oPort[devh1].noofreplay > g_ivrnoretry - 1 ){
					oPort[devh1].state = ST_ENDCALL;
					nFiledesc = endcall_efh;
				}
				else{
					oPort[devh1].noofreplay = oPort[devh1].noofreplay + 1;
					nFiledesc = acksms_efh;
				}
			}
		}
		break;

   } /* end switch */
   
	if( dx_fileseek( nFiledesc, 0, SEEK_SET ) == -1 ){
	   sprintf( tmp, "dx_fileseek( %d, 0, SEEK_SET ) return -1", nFiledesc );
	   sys_update_log( devh1, LOG_ERR, tmp );
   }

   /*  Clear and Set-Up the IOTT strcuture    */
   memset(oPort[devh1].iott, 0, sizeof( DX_IOTT ) );

   oPort[devh1].iott[0].io_type = IO_DEV | IO_EOT;
   oPort[devh1].iott[0].io_fhandle = nFiledesc;
   oPort[devh1].iott[0].io_offset = 0;
   oPort[devh1].iott[0].io_length = -1;

   /*  Clear and then Set the DV_TPT structures */
   dx_clrtpt( tpt, 1 );

   /* Terminate Play on Receiving any DTMF tone */
   tpt[ 0 ].tp_type = IO_EOT;
   tpt[ 0 ].tp_termno = DX_MAXDTMF;
   tpt[ 0 ].tp_length = 1;
   tpt[ 0 ].tp_flags = TF_MAXDTMF | TF_CLRBEG ;

   /* Play VOX File on D/160 Channel, Normal Play Back */
   if( dx_play( oPort[devh1].voiceh, oPort[devh1].iott,tpt, EV_ASYNC | PM_ALAW | PM_SR8 ) == -1 ){
      strcpy( tmp, "dx_play( oPort[devh1].voiceh, oPort[devh1].iott,tpt, EV_ASYNC ) return -1");
	  sys_update_log( devh1, LOG_ERR, tmp );
	  sprintf( tmp, "Error on the device was 0x%x", ATDV_LASTERR( oPort[devh1].voiceh ) );
	  sys_update_log( devh1, LOG_ERR, tmp );

   }

}

void getdigs(int devh)
{
	char tmp[127];
	int devh1 = devh/2;
	
	memset( tpt, 0, (sizeof( DV_TPT ) * 2) );

	/* Terminate GetDigits on Receiving MAXDTMF Digits */
	tpt[ 0 ].tp_type   = IO_CONT;
	tpt[ 0 ].tp_termno = DX_MAXDTMF;
	tpt[ 0 ].tp_length = 1;
	tpt[ 0 ].tp_flags  = TF_MAXDTMF;

	/* Terminate GetDigits after 8 Seconds */
	tpt[ 1 ].tp_type   = IO_EOT;
	tpt[ 1 ].tp_termno = DX_MAXTIME;
	tpt[ 1 ].tp_length = 20 * g_ivrperdig;
	tpt[ 1 ].tp_flags  = TF_MAXTIME;

	if( dx_getdig( oPort[devh1].voiceh, tpt, &(oPort[devh1].digbuf), EV_ASYNC ) == -1 ){
		sprintf( tmp, "dx_getdig( oPort[%d].voiceh, tpt, &(oPort[%d].digbuf), EV_ASYNC ) return -1", devh1, devh1 );
		sys_update_log( devh1, LOG_ERR, tmp );
	}
	
}

void dropcall(int devh)
{
	int nRetval;
	int devh1 = devh/2;
	char tmp[127];

	nRetval = gc_DropCall( oPort[devh1].crn, GC_NORMAL_CLEARING, EV_ASYNC );
	if (nRetval != EGC_NOERR) {
		sprintf( tmp, "gc_DropCall( oPort[devh1].crn, GC_NORMAL_CLEARING, EV_ASYNC ) return %d", nRetval );
		sys_update_log( devh1, LOG_ERR, tmp );
	}
}

void getnric(int devh)
{
	char tmp[127];
	int devh1 = devh/2;
	/*
	 * Clear and then Set the DV_TPT structures
	 */
	memset( tpt, 0, (sizeof( DV_TPT ) * 3) );

	/* Terminate GetDigits on Receiving MAXDTMF Digits */
	tpt[ 0 ].tp_type   = IO_CONT;
	tpt[ 0 ].tp_termno = DX_DIGMASK;
	tpt[ 0 ].tp_length = DM_P;
	tpt[ 0 ].tp_flags  = TF_LEVEL;

	/* Terminate GetDigits on Receiving MAXDTMF Digits */
	tpt[ 1 ].tp_type   = IO_CONT;
	tpt[ 1 ].tp_termno = DX_MAXDTMF;
	tpt[ 1 ].tp_length = 25;
	tpt[ 1 ].tp_flags  = TF_MAXDTMF;

	/* Terminate GetDigits after 8 Seconds */
	tpt[ 2 ].tp_type   = IO_EOT;
	tpt[ 2 ].tp_termno = DX_MAXTIME;
	tpt[ 2 ].tp_length = 100 * g_ivrperdig;
	tpt[ 2 ].tp_flags  = TF_MAXTIME;

	if( dx_getdig( oPort[devh1].voiceh, tpt, &(oPort[devh1].digbuf), EV_ASYNC ) == -1 ){
		sprintf( tmp, "dx_getdig( oPort[%d].voiceh, tpt, &(oPort[%d].digbuf), EV_ASYNC ) return -1", devh1, devh1 );
		sys_update_log( devh1, LOG_ERR, tmp );
	}
}

void chknric(int devh)
{
	int devh1, digit_length = 0;
	char digits[50], tmp[127];
	int i;

	devh1 = devh/2;
	strcpy( digits, oPort[devh1].digbuf.dg_value );
	sprintf( tmp, "Get digits completed %s", digits );
	sys_update_log( devh1, LOG_EVT, tmp );

	digit_length = strcspn( digits, "#" );
	strcpy( oPort[devh1].cCalldigits, digits );

	for( i=0; i<digit_length; i++ ){
		if( digits[i] == '*' ){
			digit_length = 0;
			break;
		}
	}

	if( digit_length > 19 || digit_length < 5 ){
		
		if( oPort[devh1].noofpin >= g_ivrnoretry - 1 ){
			oPort[devh1].state = ST_MAXRETRY;
			(*playlong)( devh );
			return;
		}
		else{
			oPort[devh1].noofpin = oPort[devh1].noofpin + 1 ;
			if( strlen( digits ) == 0 ){
				oPort[devh1].state = ST_NONRIC;
				(*playlong)( devh );
			}
			else{
				oPort[devh1].state = ST_WRONGNRIC;
				(*playlong)( devh );
			}
		}
	}
	else{
		digits[digit_length] ='\0';
		strcpy( oPort[devh1].playnumber, digits );
		strcpy( oPort[devh1].cPin, digits );

		oPort[devh1].state = ST_REPEATNRIC;
		(*playlong)( devh );
	}
}

void chkdig(int devh)
{
	int devh1 = devh/2;
	int playno, length;
	char digits[21];
	char digit;
	
	if( oPort[devh1].playno < 0 || oPort[devh1].playno > 20 )
		oPort[devh1].playno = 0;
	playno = oPort[devh1].playno;
	// 2009-12-08 DELL SERVER FAILOVER
	if( oPort[devh1].state == ST_ACK1 && playno == 0 ){
		update_mobnominalroll( oPort[devh1].nMobid, 0, oPort[devh1].cPin, 'I', devh1 );
	}
			
	oPort[devh1].playnumber[20] = '\0';
	length = strlen( oPort[devh1].playnumber );
	strcpy( digits, oPort[devh1].playnumber );
	digit = digits[playno] ;
	if( playno <= length - 1 ){
		switch( digit ){
		case '0':
			oPort[devh1].state = ST_N0;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;
		
		case '1':
			oPort[devh1].state = ST_N1;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case '2':
			oPort[devh1].state = ST_N2;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case '3':
			oPort[devh1].state = ST_N3;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case '4':
			oPort[devh1].state = ST_N4;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case '5':
			oPort[devh1].state = ST_N5;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case '6':
			oPort[devh1].state = ST_N6;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case '7':
			oPort[devh1].state = ST_N7;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case '8':
			oPort[devh1].state = ST_N8;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case '9':
			oPort[devh1].state = ST_N9;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;
		
		case 'a':
			oPort[devh1].state = ST_N10;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;
		
		case 'b':
			oPort[devh1].state = ST_N11;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'c':
			oPort[devh1].state = ST_N12;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'd':
			oPort[devh1].state = ST_N13;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'e':
			oPort[devh1].state = ST_N14;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'f':
			oPort[devh1].state = ST_N15;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'g':
			oPort[devh1].state = ST_N16;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'h':
			oPort[devh1].state = ST_N17;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'i':
			oPort[devh1].state = ST_N18;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'j':
			oPort[devh1].state = ST_N19;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;
		
		case 'k':
			oPort[devh1].state = ST_N20;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;
		
		case 'u':
			oPort[devh1].state = ST_N30;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;
		
		case 'v':
			oPort[devh1].state = ST_N40;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'w':
			oPort[devh1].state = ST_N50;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'x':
			oPort[devh1].state = ST_N60;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'y':
			oPort[devh1].state = ST_N70;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'z':
			oPort[devh1].state = ST_N80;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'A':
			oPort[devh1].state = ST_N90;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'B':
			oPort[devh1].state = ST_N100;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'C':
			oPort[devh1].state = ST_N1000;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case 'M':
			oPort[devh1].state = ST_DOLLAR;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;
		case 'N':
			oPort[devh1].state = ST_DOLLARS;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;
		case 'O':
			oPort[devh1].state = ST_CENT;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;
		case 'P':
			oPort[devh1].state = ST_CENTS;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		case '.':
			oPort[devh1].state = ST_AND;
			oPort[devh1].playno = oPort[devh1].playno + 1;
			(*playlong)( devh );
			break;

		default:
			oPort[devh1].state = ST_EMPTY;
			(*playlong)( devh );
		}
	}
	else{
		switch( oPort[devh1].lststate ){
		case ST_REPEATNRIC:
			oPort[devh1].state = ST_CONFIRMDIG;
			oPort[devh1].playno = 0;
			(*play)( devh );
			break;
		case ST_REPEATMOBILE:
			oPort[devh1].state = ST_CONFIRMMNO;
			oPort[devh1].playno = 0;
			(*play)( devh );
			break;
		
		case ST_ACK1:
			oPort[devh1].state = ST_ACK2;
			oPort[devh1].playno = 0;
			(*playlong)( devh );
			break;
		default:
			break;
		}
	}
}

void playlong(int devh)
{
   int nFiledesc;
   char tmp[127];
   int devh1;
	
   devh1 = devh/2;

   /* Seach Play Message */
	switch(oPort[devh1].state){

	case ST_SYSNOUSE:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = sysnouse_cfh;
		else
			nFiledesc = sysnouse_efh;
		break;

	case ST_WELCOME:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = welcome_cfh;
		else
			nFiledesc = welcome_efh;
		break;

	case ST_LANGUAGE:
	   nFiledesc = language_fh;
	   break;
   
	case ST_NOLANGUAGE:
	   nFiledesc = nolanguage_fh;
	   break;

    case ST_WRONGLANGUAGE:
	   nFiledesc = wronglanguage_fh;
	   break;

	case ST_ACKNOWLEDGED:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = acknowledged_cfh;
		else
			nFiledesc = acknowledged_efh;
		break;

	case ST_DELETED:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = deleted_cfh;
		else
			nFiledesc = deleted_efh;
		break;

	case ST_NONRIC:
	case ST_NOSERVICE:
	case ST_NOCONFIRM:
	case ST_NOACKSMS:
	case ST_NOMOBILENO:
	case ST_NOCONFMNO:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = noentry_cfh;
		else
			nFiledesc = noentry_efh;
		break;

	case ST_MAXRETRY:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = maxretry_cfh;
		else
			nFiledesc = maxretry_efh;
		break;

	case ST_WRONGNRIC:
		if( oPort[devh1].nLanguage == 2 ){
			nFiledesc = wrongic_cfh;
			if( oPort[devh1].nICtype == 1 )
				nFiledesc = wrongnric_cfh;
			if( oPort[devh1].nICtype == 2 )
				nFiledesc = wrongfin_cfh;
			if( oPort[devh1].nICtype == 3 )
				nFiledesc = wrongpassport_cfh;			
		}
		else{
			nFiledesc = wrongic_efh;
			if( oPort[devh1].nICtype == 1 )
				nFiledesc = wrongnric_efh;
			if( oPort[devh1].nICtype == 2 )
				nFiledesc = wrongfin_efh;
			if( oPort[devh1].nICtype == 3 )
				nFiledesc = wrongpassport_efh;
		}
		break;
	case ST_WRONGSERVICE:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = wrongservice_cfh;
		else
			nFiledesc = wrongservice_efh;
		break;
	
	case ST_WRONGMOBILENO:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = wrongmobileno_cfh;
		else
			nFiledesc = wrongmobileno_efh;
		break;
	
	case ST_WRONGCONFIRM:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = wrongdigit_cfh;
		else
			nFiledesc = wrongdigit_efh;
		break;
	case ST_WRONGCONFMNO:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = wrongdigit_cfh;
		else
			nFiledesc = wrongdigit_efh;
		break;

	case ST_ENDCALL:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = endcall_cfh;
		else
			nFiledesc = endcall_efh;
		break;

	case ST_REPEATNRIC:
		oPort[devh1].lststate = ST_REPEATNRIC;
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = repeatnric_cfh;
		else
			nFiledesc = repeatnric_efh;
		break;
	
	case ST_REPEATMOBILE:
		oPort[devh1].lststate = ST_REPEATMOBILE;
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = repeatnric_cfh;
		else
			nFiledesc = repeatnric_efh;
		break;
		
	case ST_NOTMOB:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = notmob_cfh;
		else
			nFiledesc = notmob_efh;
		break;

	case ST_RECVSMS:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = recvsms_cfh;
		else
			nFiledesc = recvsms_efh;
		break;

	case ST_MOBILENO:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = mobileno_cfh;
		else
			nFiledesc = mobileno_efh;
		break;

	case ST_N0:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n0_cfh;
		else
			nFiledesc = n0_efh;
		break;

	case ST_N1:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n1_cfh;
		else
			nFiledesc = n1_efh;
		break;

	case ST_N2:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n2_cfh;
		else
			nFiledesc = n2_efh;
		break;
		
	case ST_N3:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n3_cfh;
		else
			nFiledesc = n3_efh;
		break;
		
	case ST_N4:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n4_cfh;
		else
			nFiledesc = n4_efh;
		break;
		
	case ST_N5:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n5_cfh;
		else
			nFiledesc = n5_efh;
		break;
		
	case ST_N6:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n6_cfh;
		else
			nFiledesc = n6_efh;
		break;
			
	case ST_N7:	
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n7_cfh;
		else
			nFiledesc = n7_efh;
		break;
		
	case ST_N8:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n8_cfh;
		else
			nFiledesc = n8_efh;
		break;
		
	case ST_N9:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n9_cfh;
		else
			nFiledesc = n9_efh;
		break;
		
	case ST_N10:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n10_cfh;
		else
			nFiledesc = n10_efh;
		break;

	case ST_N11:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n11_cfh;
		else
			nFiledesc = n11_efh;
		break;

	case ST_N12:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n12_cfh;
		else
			nFiledesc = n12_efh;
		break;
		
	case ST_N13:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n13_cfh;
		else
			nFiledesc = n13_efh;
		break;
		
	case ST_N14:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n14_cfh;
		else
			nFiledesc = n14_efh;
		break;
		
	case ST_N15:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n15_cfh;
		else
			nFiledesc = n15_efh;
		break;
		
	case ST_N16:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n16_cfh;
		else
			nFiledesc = n16_efh;
		break;
			
	case ST_N17:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n17_cfh;
		else
			nFiledesc = n17_efh;
		break;
		
	case ST_N18:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n18_cfh;
		else
			nFiledesc = n18_efh;
		break;
		
	case ST_N19:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n19_cfh;
		else
			nFiledesc = n19_efh;
		break;
	
	case ST_N20:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n20_cfh;
		else
			nFiledesc = n20_efh;
		break;

	case ST_N30:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n30_cfh;
		else
			nFiledesc = n30_efh;
		break;

	case ST_N40:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n40_cfh;
		else
			nFiledesc = n40_efh;
		break;

	case ST_N50:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n50_cfh;
		else
			nFiledesc = n50_efh;
		break;
		
	case ST_N60:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n60_cfh;
		else
			nFiledesc = n60_efh;
		break;
		
	case ST_N70:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n70_cfh;
		else
			nFiledesc = n70_efh;
		break;
		
	case ST_N80:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n80_cfh;
		else
			nFiledesc = n80_efh;
		break;
		
	case ST_N90:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n90_cfh;
		else
			nFiledesc = n90_efh;
		break;
			
	case ST_N100:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n100_cfh;
		else
			nFiledesc = n100_efh;
		break;
		
	case ST_N1000:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = n1000_cfh;
		else
			nFiledesc = n1000_efh;
		break;
		
	case ST_CENT:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = cent_cfh;
		else
			nFiledesc = cent_efh;
		break;
	
	case ST_CENTS:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = cents_cfh;
		else
			nFiledesc = cents_efh;
		break;
	
	case ST_DOLLAR:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = dollar_cfh;
		else
			nFiledesc = dollar_efh;
		break;
	
	case ST_DOLLARS:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = dollars_cfh;
		else
			nFiledesc = dollars_efh;
		break;
	
	case ST_AND:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = and_cfh;
		else
			nFiledesc = and_efh;
		break;

	case ST_ACK1:
		oPort[devh1].lststate = ST_ACK1;
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = ack1_cfh;
			//msgfh[devh1] = dx_fileopen( "ACKNOWLEDGE1c.VOX", O_RDONLY|O_BINARY );
		else
			nFiledesc = ack1_efh;
			//msgfh[devh1] = dx_fileopen( "ACKNOWLEDGE1e.VOX", O_RDONLY|O_BINARY );
		//nFiledesc = msgfh[devh1];
		break;
		
	case ST_ACK2:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = ack2_cfh;
			//msgfh[devh1] = dx_fileopen( "ACKNOWLEDGE2c.VOX", O_RDONLY|O_BINARY );
		else
			nFiledesc = ack2_efh;
			//msgfh[devh1] = dx_fileopen( "ACKNOWLEDGE2e.VOX", O_RDONLY|O_BINARY );
		//nFiledesc = msgfh[devh1];
		break;

	case ST_ACKSMS:
	   if( oPort[devh1].nLanguage == 2 )
		   nFiledesc = acksms_cfh;
	   else
		   nFiledesc = acksms_efh;
	   break;

	case ST_REPEATACK:
		if( oPort[devh1].nAcksms > 0 ){ // Mobile number in the list
			if( oPort[devh1].nLanguage == 2 ){
				if( oPort[devh1].noofreplay > g_ivrnoretry - 1 ){
					oPort[devh1].state = ST_ENDCALL;
					nFiledesc = endcall_cfh;
				}
				else{
					oPort[devh1].noofreplay = oPort[devh1].noofreplay + 1;
					nFiledesc = repeat_cfh;
				}
			}
			else{
				if( oPort[devh1].noofreplay > g_ivrnoretry - 1 ){
					oPort[devh1].state = ST_ENDCALL;
					nFiledesc = endcall_efh;
				}
				else{
					oPort[devh1].noofreplay = oPort[devh1].noofreplay + 1;
					nFiledesc = repeat_efh;
				}
			}
		}
		else{
			oPort[devh1].state = ST_ACKSMS;
			if( oPort[devh1].nLanguage == 2 ){
				if( oPort[devh1].noofreplay > g_ivrnoretry - 1 ){
					oPort[devh1].state = ST_ENDCALL;
					nFiledesc = endcall_cfh;
				}
				else{
					oPort[devh1].noofreplay = oPort[devh1].noofreplay + 1;
					nFiledesc = acksms_cfh;
				}
			}
			else{
				if( oPort[devh1].noofreplay > g_ivrnoretry - 1 ){
					oPort[devh1].state = ST_ENDCALL;
					nFiledesc = endcall_efh;
				}
				else{
					oPort[devh1].noofreplay = oPort[devh1].noofreplay + 1;
					nFiledesc = acksms_efh;
				}
			}
		}
		break;

	case ST_EMPTY:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = empty_cfh;
		else
			nFiledesc = empty_efh;
		break;
	
	default:
		if( oPort[devh1].nLanguage == 2 )
			nFiledesc = empty_cfh;
		else
			nFiledesc = empty_efh;
		break;
	}  /* end switch */

	if( dx_fileseek( nFiledesc, 0, SEEK_SET ) == -1 ){
		sprintf( tmp, "dx_fileseek( %d, 0, SEEK_SET ) return -1", nFiledesc );
		sys_update_log( devh1, LOG_ERR, tmp );	
	}
	
	memset(oPort[devh1].iott, 0, sizeof( DX_IOTT ) );

	oPort[devh1].iott[0].io_type = IO_DEV | IO_EOT;
	oPort[devh1].iott[0].io_fhandle = nFiledesc;
	oPort[devh1].iott[0].io_offset = 0;
	oPort[devh1].iott[0].io_length = -1;
	
	dx_clrtpt( tpt, 1 );
	
	/* Terminate Play on Receiving any DTMF tone */
	tpt[ 0 ].tp_type = IO_EOT;
	tpt[ 0 ].tp_termno = DX_MAXDTMF;
	tpt[ 0 ].tp_length = 10;
	tpt[ 0 ].tp_flags = TF_MAXDTMF|TF_CLRBEG;
	
	/* Play VOX File on D/160 Channel, Normal Play Back */
	if( dx_play( oPort[devh1].voiceh, oPort[devh1].iott,tpt, EV_ASYNC | PM_ALAW | PM_SR8 ) == -1 ){
		sprintf( tmp, "dx_play( oPort[%d].voiceh, oPort[devh1].iott,tpt, EV_ASYNC ) return -1", devh1 );
		sys_update_log( devh1, LOG_ERR, tmp );
		sprintf( tmp, "Error on the device was 0x%x\n", ATDV_LASTERR( oPort[devh1].voiceh ) );
		sys_update_log( devh1, LOG_ERR, tmp );
	}
	/*else{  2009-12-08 DELL SERVER FAILOVER
		if( oPort[devh1].state == ST_ACK1 ){
			update_mobnominalroll( oPort[devh1].nMobid, 0, oPort[devh1].cPin, 'I', devh1 );
		}
	}  2009-12-08 DELL SERVER FAILOVER */
}

void chkconfirm(int devh)
{
	int devh1 = devh/2;
	char digits[50], tmp[127];
	char cdata[20], cOutstanding[20];
	int nRetval;
	char cStatement[500];
	int i, n;
	int dollars = 0;
	int cents = 0;

	SQLHSTMT hstmt;
	SQLCHAR cAckstatus[2],cBroreset[2];
	SQLINTEGER cb_cAckstatus = SQL_NTS, cb_cBroreset = SQL_NTS;
	SQLCHAR cNRIC[21],cRegistercode[5];
	SQLINTEGER cb_cNRIC = SQL_NTS, cb_cRegistercode = SQL_NTS;
	SQLCHAR cAckfile1[10],cAckfile2[10];
	SQLINTEGER cb_cAckfile1 = SQL_NTS, cb_cAckfile2 = SQL_NTS;
	SQLINTEGER nMobid = 0, cb_nMobid = 0;
	SQLINTEGER nPriority = 0, cb_nPriority = 0;
	SQLINTEGER nWhichbroadcast = 0, cb_nWhichbroadcast = 0;
	//char cMobdttm[25];
	SQLINTEGER cb_cMobdttm = SQL_NTS;
	//char cMonth[3], cDay[3], cTime1[5];
	SQLDOUBLE nOutstanding = 0;
	SQLINTEGER cb_nOutstanding = 0;
	
	strcpy( digits, oPort[devh1].digbuf.dg_value );
	sprintf( tmp, "Get digits completed %s", digits );
	sys_update_log( devh1, LOG_EVT, tmp );

	if( hdbc == NULL ){
		oPort[devh1].state = ST_NOTMOB;
		(*playlong)( devh );
		return;
	}

	switch( digits[0] ){
	case '1':
		oPort[devh1].cAckstatus = 'F';
		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return;
		}

		strcpy( cStatement, "select mob_id,whichbroadcast,nric,priority,registercode,ack_status,bro_reset,a_voxfile1,a_voxfile2,outstanding from mobnominalroll (nolock) where pin = '");
		strcat( cStatement, oPort[devh1].cPin );
		strcat( cStatement, "' and excuse_flag = 'N'");
		sys_update_log( devh1, LOG_SQL, cStatement );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nMobid,0,&cb_nMobid);
		nRetval = SQLBindCol(hstmt,2,SQL_C_SLONG,&nWhichbroadcast,0,&cb_nWhichbroadcast);
		nRetval = SQLBindCol(hstmt,3,SQL_C_CHAR,&cNRIC,21,&cb_cNRIC);
		nRetval = SQLBindCol(hstmt,4,SQL_C_SLONG,&nPriority,0,&cb_nPriority);
		nRetval = SQLBindCol(hstmt,5,SQL_C_CHAR,&cRegistercode,5,&cb_cRegistercode);
		nRetval = SQLBindCol(hstmt,6,SQL_C_CHAR,&cAckstatus,2,&cb_cAckstatus);
		nRetval = SQLBindCol(hstmt,7,SQL_C_CHAR,&cBroreset,2,&cb_cBroreset);
		nRetval = SQLBindCol(hstmt,8,SQL_C_CHAR,&cAckfile1,10,&cb_cAckfile1);
		nRetval = SQLBindCol(hstmt,9,SQL_C_CHAR,&cAckfile2,10,&cb_cAckfile2);
		nRetval = SQLBindCol(hstmt,10,SQL_C_DOUBLE,&nOutstanding,sizeof(SQLDOUBLE),&cb_nOutstanding);
	
		nRetval = SQLFetch(hstmt);
		if( nRetval != SQL_SUCCESS && nRetval != SQL_SUCCESS_WITH_INFO ){
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
//			oPort[devh1].state = ST_NOTMOB;
//			(*playlong)( devh );
			if( oPort[devh1].noofpin >= g_ivrnoretry - 1 ){
				oPort[devh1].state = ST_MAXRETRY;
				(*playlong)( devh );
			}
			else{
				oPort[devh1].noofpin = oPort[devh1].noofpin + 1;
				oPort[devh1].state = ST_WRONGNRIC;
				(*playlong)( devh );
			}
			return;
		}
		nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
		nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		
//		if( oPort[devh1].switched == OUTGOING ){
//			if( oPort[devh1].cRegistercode[0] == cRegistercode[0] && oPort[devh1].cRegistercode[1] == cRegistercode[1] &&
//				oPort[devh1].cRegistercode[2] == cRegistercode[2] && oPort[devh1].cRegistercode[3] == cRegistercode[3] ){
//			}
//			else{
//				if( oPort[devh1].noofretry >= g_ivrnoretry - 1 ){
//					oPort[devh1].state = ST_MAXRETRY;
//					(*playlong)( devh );
//				}
//				else{
//					oPort[devh1].noofretry = oPort[devh1].noofretry + 1 ;
//					oPort[devh1].state = ST_WRONGNRIC;
//					(*playlong)( devh );
//				}
//				return;
//			}
//		}
		cNRIC[20] = '\0';
		oPort[devh1].nMobid = nMobid;
		oPort[devh1].nWhichbroadcast = nWhichbroadcast;
		strcpy( oPort[devh1].cNRIC, cNRIC );
		oPort[devh1].nPriority = nPriority;
		strcpy( oPort[devh1].cRegistercode, cRegistercode );
		
		strcpy( cOutstanding, "" );
		sprintf( cdata, "%.2f", nOutstanding );
		strcpy( tmp, "" );
		n = strlen( cdata );
		if( n < 4 && n > 7 ){
			strcpy( oPort[devh1].cOutstanding, "" );
		}
		else{
			for(i=0; i<20; i++){
				cOutstanding[i] = '\0';
			}
			for(i=0; i<n-3; i++){
				tmp[i] = cdata[i];
			}
			tmp[i] = '\0';
			dollars = atoi( tmp );
			tmp[0] = cdata[n-2];
			tmp[1] = cdata[n-1];
			tmp[2] = '\0';
			cents = atoi( tmp );

			i = 0;
			sprintf( cdata, "%.4d", dollars );
			if( cdata[0] == '0' ){
			}
			else{
				cOutstanding[i] = cdata[0];
				i = i + 1;
				cOutstanding[i] = 'C';
				i = i + 1;
			}
			if( cdata[1] == '0' ){
			}
			else{
				cOutstanding[i] = cdata[1];
				i = i + 1;
				cOutstanding[i] = 'B';
				i = i + 1;				
			}
			if( cdata[2] == '0' ){
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '1' ){
				if( cdata[3] == '0' )
					cOutstanding[i] = 'a';
				if( cdata[3] == '1' )
					cOutstanding[i] = 'b';
				if( cdata[3] == '2' )
					cOutstanding[i] = 'c';
				if( cdata[3] == '3' )
					cOutstanding[i] = 'd';
				if( cdata[3] == '4' )
					cOutstanding[i] = 'e';
				if( cdata[3] == '5' )
					cOutstanding[i] = 'f';
				if( cdata[3] == '6' )
					cOutstanding[i] = 'g';
				if( cdata[3] == '7' )
					cOutstanding[i] = 'h';
				if( cdata[3] == '8' )
					cOutstanding[i] = 'i';
				if( cdata[3] == '9' )
					cOutstanding[i] = 'j';
				i = i + 1;
			}
			else if( cdata[2] == '2' ){
				cOutstanding[i] = 'k';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '3' ){
				cOutstanding[i] = 'u';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '4' ){
				cOutstanding[i] = 'v';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '5' ){
				cOutstanding[i] = 'w';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '6' ){
				cOutstanding[i] = 'x';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '7' ){
				cOutstanding[i] = 'y';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '8' ){
				cOutstanding[i] = 'z';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '9' ){
				cOutstanding[i] = 'A';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else{
			}
			
			if( dollars == 0 ){
			}
			else if( dollars == 1 ){
				cOutstanding[i] = 'M';
				i = i + 1;
			}
			else{
				cOutstanding[i] = 'N';
				i = i + 1;
			}

			if( dollars > 0 && cents > 0 ){
				cOutstanding[i] = '.';
				i = i + 1;
			}

			strcpy( cdata, "" );
			sprintf( cdata, "%.4d", cents );
			if( cdata[2] == '0' ){
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '1' ){
				if( cdata[3] == '0' )
					cOutstanding[i] = 'a';
				if( cdata[3] == '1' )
					cOutstanding[i] = 'b';
				if( cdata[3] == '2' )
					cOutstanding[i] = 'c';
				if( cdata[3] == '3' )
					cOutstanding[i] = 'd';
				if( cdata[3] == '4' )
					cOutstanding[i] = 'e';
				if( cdata[3] == '5' )
					cOutstanding[i] = 'f';
				if( cdata[3] == '6' )
					cOutstanding[i] = 'g';
				if( cdata[3] == '7' )
					cOutstanding[i] = 'h';
				if( cdata[3] == '8' )
					cOutstanding[i] = 'i';
				if( cdata[3] == '9' )
					cOutstanding[i] = 'j';
				i = i + 1;
			}
			else if( cdata[2] == '2' ){
				cOutstanding[i] = 'k';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '3' ){
				cOutstanding[i] = 'u';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '4' ){
				cOutstanding[i] = 'v';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '5' ){
				cOutstanding[i] = 'w';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '6' ){
				cOutstanding[i] = 'x';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '7' ){
				cOutstanding[i] = 'y';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '8' ){
				cOutstanding[i] = 'z';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else if( cdata[2] == '9' ){
				cOutstanding[i] = 'A';
				i = i + 1;
				if( cdata[3] == '0' ){
				}
				else{
					cOutstanding[i] = cdata[3];
					i = i + 1;
				}
			}
			else{
			}
			
			if( cents == 0 ){
			}
			else if( cents == 1 ){
				cOutstanding[i] = 'O';
				i = i + 1;
			}
			else{
				cOutstanding[i] = 'P';
				i = i + 1;
			}

			cOutstanding[i] = '\0';
			strcpy( oPort[devh1].cOutstanding, cOutstanding );

		}
			
		strcpy( oPort[devh1].cAckfile1, cAckfile1 );
		strcpy( oPort[devh1].cAckfile2, cAckfile2 );
		oPort[devh1].cBroreset = cBroreset[0];
		
//		if( nWhichbroadcast == 0 ){
//			oPort[devh1].state = ST_DELETED;
//			(*playlong)( devh );
//			return;
//		}
		
		/*SQLAllocHandle( SQL_HANDLE_STMT, hdbc,&hstmt );
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return;
		}

		sprintf(cStatement,"select mob_str_dttm from mobbroadcast_timetable where mob_id = %d", nMobid );
		sys_update_log( devh1, LOG_SQL, cStatement );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)&cStatement,SQL_NTS);
		if ( nRetval != SQL_SUCCESS && nRetval != SQL_SUCCESS_WITH_INFO ){
			sys_update_log( 0, LOG_ERR, cStatement );
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
			strcpy(cDay, "**" );
			strcpy(cMonth, "**" );
			strcpy(cTime1, "****" );
		}
		else{
			nRetval = SQLBindCol(hstmt,1,SQL_C_CHAR,&cMobdttm,25,&cb_cMobdttm);
			nRetval = SQLFetch( hstmt );
			SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
			if( nRetval == SQL_SUCCESS || nRetval == SQL_SUCCESS_WITH_INFO ){
		
				cMobdttm[19] = '\0';
				cDay[0] = cMobdttm[8];
				cDay[1] = cMobdttm[9];
				cDay[2] = '\0';
				cMonth[0] = cMobdttm[5];
				cMonth[1] = cMobdttm[6];
				cMonth[2] = '\0';
				cTime1[0] = cMobdttm[11];
				cTime1[1] = cMobdttm[12];
				cTime1[2] = cMobdttm[14];
				cTime1[3] = cMobdttm[15];
				cTime1[4] = '\0';
			}
			else{
				strcpy(cDay, "**" );
				strcpy(cMonth, "**" );
				strcpy(cTime1, "****" );
			}
		}
		strcpy( oPort[devh1].cDay, cDay );
		strcpy( oPort[devh1].cMonth, cMonth );
		strcpy( oPort[devh1].cTime1, cTime1 ); */

		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return;
		}
		sprintf(cStatement,"select mob_id from mobnominalroll (nolock) where pin = '%s' and (cn1st = '%s' or cn2nd = '%s' or cn3rd = '%s' or cn4th = '%s' or cn5th = '%s' or cn6th = '%s')", 
			oPort[devh1].cPin, oPort[devh1].cContactno, oPort[devh1].cContactno, oPort[devh1].cContactno, oPort[devh1].cContactno, oPort[devh1].cContactno, oPort[devh1].cContactno );
		sys_update_log( devh1, LOG_SQL, cStatement );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nMobid,0,&cb_nMobid);
		
		nRetval = SQLFetch(hstmt);
		if( nRetval != SQL_SUCCESS && nRetval != SQL_SUCCESS_WITH_INFO ){
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
			oPort[devh1].nAcksms = 0;
		}
		else{
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
			if( oPort[devh1].cContactno[0] == '8' || oPort[devh1].cContactno[0] == '9' )
				oPort[devh1].nAcksms = 1;
			else
				oPort[devh1].nAcksms = 0;
		}
		

		switch( cAckstatus[0] ){
		case 'D':
			oPort[devh1].state = ST_DELETED;
			(*playlong)( devh );
			break;
		case 'I':
		case 'S':
		case 'W':
//			oPort[devh1].cAckstatus = 'N';
//			oPort[devh1].state = ST_ACKNOWLEDGED;
//			(*playlong)( devh );
//			break;
		case 'N':
			oPort[devh1].cAckstatus = 'S';
			oPort[devh1].state = ST_ACK1;

			strcpy( oPort[devh1].playnumber, oPort[devh1].cOutstanding );
			oPort[devh1].playno = 0;
			(*playlong)( devh );
			break;
		default:
			break;
		}
		
		break;

	case '0':
		if( oPort[devh1].noofretry > g_ivrnoretry - 1 ){
			oPort[devh1].state = ST_MAXRETRY;
			(*playlong)( devh );
		}
		else{
			oPort[devh1].noofretry = oPort[devh1].noofretry + 1;
			oPort[devh1].state = ST_NRIC;
			(*play)( devh );
		}
		break;

	case '3':
		if( oPort[devh1].noofretry > g_ivrnoretry - 1 ){
			oPort[devh1].state = ST_MAXRETRY;
			(*playlong)( devh );
		}
		else{
			oPort[devh1].noofretry = oPort[devh1].noofretry + 1;
			oPort[devh1].playno = 0;
			oPort[devh1].state = ST_REPEATNRIC;
			(*playlong)( devh );
		}
		break;

	case '9':
		(*dropcall)( devh );
		break;

	default:
		if( strlen( oPort[devh1].digbuf.dg_value ) > 0 ){
			if( oPort[devh1].noofretry > g_ivrnoretry - 1 ){
				oPort[devh1].state = ST_MAXRETRY;
				(*playlong)( devh );
			}
			else{
				oPort[devh1].noofretry = oPort[devh1].noofretry + 1;
				oPort[devh1].state = ST_WRONGCONFIRM;
				(*playlong)( devh );
				
			}
		}
		else{
			if( oPort[devh1].noofretry > g_ivrnoretry - 1 ){
				oPort[devh1].state = ST_MAXRETRY;
				(*playlong)(devh);
			}
			else{
				oPort[devh1].noofretry = oPort[devh1].noofretry + 1;
				oPort[devh1].state = ST_NOCONFIRM;
				(*playlong)( devh );
			}
		}
		break;
	}
}

void playdig(int devh)
{
	int devh1 = devh/2;
	DX_CAP cap;
	char tmp[127];
	
	dx_clrcap( &cap );
	cap.ca_nbrdna = 3;
	cap.ca_nbrbeg = 2;
	
	strcpy( m_cDialdigits, "*68440844**" );
	if( dx_dial( oPort[devh1].voiceh, m_cDialdigits, &cap, EV_ASYNC ) == -1 ) {
		sprintf( tmp, "dx_dial( oPort[%d].voiceh, %s, &cap, EV_ASYNC ) return -1", devh1, m_cDialdigits );
		sys_update_log( devh1, LOG_ERR, tmp );
	
		sprintf( tmp, "ATDV_LASTERR(oPort[devh1].voiceh) return %d", ATDV_LASTERR(oPort[devh1].voiceh) );
		sys_update_log( devh1, LOG_ERR, tmp );
	
		(*dropcall)( devh );
	}
}


int cmd_open_database(void)
{
	int nRetval;

	sys_update_log( 1, LOG_EVT, "Opening Database" );
	if(SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv))){	
		(void) SQLSetEnvAttr(henv,SQL_ATTR_ODBC_VERSION,
			(SQLPOINTER)SQL_OV_ODBC3,SQL_IS_INTEGER);
	}
	else{
		hdbc = NULL;
		return FALSE;
	}
	nRetval = SQLAllocHandle(SQL_HANDLE_DBC,henv,&hdbc);
	if(nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR){
		hdbc = NULL;
		return FALSE;
	}

	nRetval = SQLDriverConnect(hdbc,NULL,(SQLCHAR*)"DSN=SPF_RECALL;UID=SQLRECALL;PWD=sqlreca11;",
		SQL_NTS,NULL,0,NULL,SQL_DRIVER_NOPROMPT);
	if( nRetval != SQL_SUCCESS && nRetval != SQL_SUCCESS_WITH_INFO ){
		hdbc = NULL;
		return FALSE;
	}
	nRetval = SQLSetConnectAttr(hdbc,SQL_ATTR_AUTOCOMMIT,SQL_AUTOCOMMIT_OFF,0);
	if(nRetval != SQL_SUCCESS){
		hdbc = NULL;
		return FALSE;
	}

	sys_update_log( 1, LOG_EVT, "Open Database Completed" );
	return TRUE;
}

int cmd_close_database()
{
	int nRetval;
	if( hdbc == NULL )
		return TRUE;

	nRetval = SQLDisconnect(hdbc);
	nRetval = SQLFreeHandle(SQL_HANDLE_DBC,hdbc);
	nRetval = SQLFreeHandle(SQL_HANDLE_ENV,henv);
	hdbc = NULL;

	return TRUE;
}

int update_station_register(int nmode)
{
	SQLHSTMT hstmt;
	int nRetval;
	char cStatement[350];
	int nChansblock = 0, i;

	if( hdbc == NULL )
		return FALSE;

	if( nmode == IN_DTTM ){
		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return FALSE;
		}

		strcpy( cStatement, "update systemmonitoring set app_name = 'ISDN', pc_name = '" );
		strcat( cStatement, m_cPcname );
		strcat( cStatement, "', in_dttm = getdate(), alarm_dttm = dateadd(n,5,getdate()), chans_block = 0, chans_dttm = getdate(), chans_alarm = dateadd(n,5,getdate()) where app_code = '" );
		strcat( cStatement, APP_CODE );
		strcat( cStatement, "'" );
		sys_update_log( 0, LOG_SQL, cStatement );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		if( nRetval != SQL_SUCCESS && nRetval != SQL_SUCCESS_WITH_INFO )
			sys_update_log( 0, LOG_ERR, cStatement );
		nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
		nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
	}

	if( nmode == UPDATE_DTTM ){
		for( i = 1; i <= m_total_chans; i++ ){
			if( oPort[i].blocked == YES ){
				nChansblock ++;
			}
		}
		if( nChansblock > 0 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return FALSE;
			}

			sprintf( cStatement, "update systemmonitoring set pro_dttm = getdate(), alarm_dttm = dateadd(n,5,getdate()), chans_block = %d where app_code = '%s'",
				nChansblock, APP_CODE );
			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			if( nRetval != SQL_SUCCESS && nRetval != SQL_SUCCESS_WITH_INFO )
				sys_update_log( 0, LOG_ERR, cStatement );
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		else{
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return FALSE;
			}

			sprintf( cStatement, "update systemmonitoring set pro_dttm = getdate(), alarm_dttm = dateadd(n,5,getdate()), chans_block = %d, chans_dttm = getdate(), chans_alarm = dateadd(n,5,getdate()) where app_code = '%s'",
				nChansblock, APP_CODE );
			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			if( nRetval != SQL_SUCCESS && nRetval != SQL_SUCCESS_WITH_INFO )
				sys_update_log( 0, LOG_ERR, cStatement );
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
	}

	if( nmode == OUT_DTTM ){
		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return FALSE;
		}

		strcpy( cStatement, "update systemmonitoring set out_dttm = getdate(), alarm_dttm = dateadd(n,5,getdate()), chans_block = 0, chans_dttm = getdate(), chans_alarm = dateadd(n,5,getdate()) where app_code = '" );
		strcat( cStatement, APP_CODE );
		strcat( cStatement, "'" );
		sys_update_log( 0, LOG_SQL, cStatement );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		if( nRetval != SQL_SUCCESS && nRetval != SQL_SUCCESS_WITH_INFO )
			sys_update_log( 0, LOG_ERR, cStatement );
		nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
		nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
	}

	return TRUE;
}

void update_mobnominalroll(int nMobid, int nWhichcontactno, char *cPin, char cStatus, int devh)
{
	SQLHSTMT hstmt;
	char cStatement[350];
	int nRetval;

	if( hdbc == NULL )
		return;

	if( cStatus == 'B' ){
		if( nWhichcontactno == 1 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn1st_status = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}

		if( nWhichcontactno == 2 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn2nd_status = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}

		if( nWhichcontactno == 3 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn3rd_status = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}

		if( nWhichcontactno == 4 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn4th_status = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}

		if( nWhichcontactno == 5 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn5th_status = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}

		if( nWhichcontactno == 6 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn6th_status = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
	}
	
	if( cStatus == 'L' ){
		if( nWhichcontactno == 1 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn1st_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 2 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn2nd_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 3 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn3rd_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 4 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn4th_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 5 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn5th_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 6 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn6th_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
	}

	if( cStatus == 'E' ){
		if( nWhichcontactno == 1 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn1st_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 2 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn2nd_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 3 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn3rd_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 4 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn4th_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 5 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn5th_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 6 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn6th_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
	}

	if( cStatus == 'R' ){
		if( nWhichcontactno == 1 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn1st_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 2 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn2nd_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 3 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn3rd_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 4 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn4th_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 5 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn5th_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 6 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set cn6th_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
	}

	if( cStatus == 'A' ){
		if( nWhichcontactno == 1 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set bro_reset = 'Y', cn1st_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 2 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set bro_reset = 'Y', cn2nd_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 3 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set bro_reset = 'Y', cn3rd_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 4 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set bro_reset = 'Y', cn4th_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 5 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set bro_reset = 'Y', cn5th_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		if( nWhichcontactno == 6 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			sprintf( cStatement, "update mobnominalroll set bro_reset = 'Y', cn6th_flag = '%c' where mob_id = %d and pin = '%s'",
				cStatus, nMobid, cPin );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}

		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return;
		}

		sprintf( cStatement, "update mobnominalroll set bro_status = 'Y', bro_dttm = getdate() where mob_id = %d and pin = '%s' and bro_status = 'N'",
			nMobid, cPin );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
		nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
	}

	if( cStatus == 'I' ){
		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return;
		}
		sprintf( cStatement, "update mobnominalroll set a_voxfile2 = '%s', ack_status = 'I', ack_dttm = getdate() where mob_id = %d and pin = '%s' and ack_status = 'N'",
			oPort[devh].cContactno, nMobid, cPin );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
		nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		
		
		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return;
		}
		if( (oPort[devh].cContactno[0] == '8' || oPort[devh].cContactno[0] == '9') && oPort[devh].nAcksms == 1 ){
			oPort[devh].nAcksms = 10; 
			sprintf( cStatement, "update mobnominalroll set register_status = 'P', register_no = '%s', noofretry = 0 where mob_id = %d and pin = '%s'",
				oPort[devh].cContactno, nMobid, cPin );
		}
		else{
			sprintf( cStatement, "update mobnominalroll set register_no = '%s', noofretry = 0 where mob_id = %d and pin = '%s'",
				oPort[devh].cContactno, nMobid, cPin );
		}
		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
		nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

	}

}

void chkdtmf(int devh)
{
}

int cmd_get_parameter()
{
	SQLHSTMT hstmt;
	SQLINTEGER isdnperpc = 0, cb_isdnperpc = 0;
	SQLINTEGER ivrperdig = 0, cb_ivrperdig = 0;
	SQLINTEGER ivrnoretry = 0, cb_ivrnoretry = 0;
	SQLINTEGER outgoingperisdn = 0, cb_outgoingperisdn = 0;
	int i, nRetval;
	char cStatement[100];

	if( hdbc == NULL )
		return FALSE;

	nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
	if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
		hdbc = NULL;
		return FALSE;
	}

	strcpy( cStatement, "select ivr_per_dig,ivr_no_retry,isdn_per_pc,outgoing_per_isdn from parameter (nolock)");
	nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
	nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&ivrperdig,0,&cb_ivrperdig);
	nRetval = SQLBindCol(hstmt,2,SQL_C_SLONG,&ivrnoretry,0,&cb_ivrnoretry);
	nRetval = SQLBindCol(hstmt,3,SQL_C_SLONG,&isdnperpc,0,&cb_isdnperpc);
	nRetval = SQLBindCol(hstmt,4,SQL_C_SLONG,&outgoingperisdn,0,&cb_outgoingperisdn);
	nRetval = SQLFetch(hstmt);
	if ( nRetval == SQL_SUCCESS || nRetval == SQL_SUCCESS_WITH_INFO ){
		g_ivrperdig = ivrperdig;
		g_ivrnoretry = ivrnoretry;
		g_isdnperpc = isdnperpc;
		g_outgoingperisdn = outgoingperisdn;
	}
	else{
		g_ivrperdig = 2;
		g_ivrnoretry = 3;
		g_isdnperpc = 2;
		g_outgoingperisdn = 25;
		cmd_close_database();
	}
	nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
	nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		
	m_total_chans = 30 * g_isdnperpc;
	m_total_outgoing = g_outgoingperisdn * g_isdnperpc;

	nRetval = 1;
	for( i = 1; i <= m_total_chans; i++ ){
		if( oPort[i].switched == INCOMING )
			nRetval = nRetval + 1;
	}
	if( nRetval < 5 ) nRetval = 5;
	if( nRetval > 10 ) nRetval = 10;
	m_total_outgoing = m_total_chans - nRetval;
	
	return TRUE;
}

void playalarm(int devh)
{
	int devh1 = devh/2;
	DX_CAP cap;
	char tmp[127];

	strcpy( m_cDialdigits, "*" );
	strcat( m_cDialdigits, oPort[devh1].cAlarmcode );
	strcat( m_cDialdigits, "**" );

	dx_clrcap(&cap);
	cap.ca_nbrdna=3;
	cap.ca_nbrbeg=2;
	
	if( dx_dial(oPort[devh1].voiceh, m_cDialdigits, &cap, EV_ASYNC) == -1 ){
		sprintf( tmp, "dx_dial(oPort[%d].voiceh, %s, &cap, EV_ASYNC) return -1", devh1, m_cDialdigits );
		sys_update_log( devh1, LOG_ERR, tmp );
		
		sprintf( tmp, "ATDV_LASTERR(oPort[devh1].voiceh) return %d", ATDV_LASTERR(oPort[devh1].voiceh) );
		sys_update_log( devh1, LOG_ERR, tmp );
	
		(*dropcall)( devh );
	}
}

void chkrepeatdig(int devh)
{
	int devh1 = devh/2;

	oPort[devh1].state = ST_ACK1;
	strcpy( oPort[devh1].playnumber, oPort[devh1].cOutstanding );
	oPort[devh1].playno = 0;
	(*playlong)( devh );

}

int update_audit_trace(int nmode)
{
	SQLHSTMT hstmt;
	int nRetval;
	char cStatement[270];
	
	if( hdbc == NULL )
		return FALSE;

	if( nmode == IN_DTTM ){
		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return FALSE;
		}

		strcpy( cStatement, "insert into audit_trace (app_name, pc_name, [function], action, action_dttm, action_by) values ('ISDN','" );
		strcat( cStatement, m_cPcname );
		strcat( cStatement, "', '" );
		strcat( cStatement, APP_CODE );
		strcat( cStatement, "', 'APP START', getdate(), '" );
		strcat( cStatement, m_cUsername );
		strcat( cStatement, "' )" );
		sys_update_log( 0, LOG_SQL, cStatement );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		if( nRetval != SQL_SUCCESS && nRetval != SQL_SUCCESS_WITH_INFO )
			sys_update_log( 0, LOG_ERR, cStatement );
		nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
		nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
	}

	if( nmode == OUT_DTTM ){
		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return FALSE;
		}

		strcpy( cStatement, "insert into audit_trace (app_name, pc_name, [function], action, action_dttm, action_by) values ('ISDN','" );
		strcat( cStatement, m_cPcname );
		strcat( cStatement, "', '" );
		strcat( cStatement, APP_CODE );
		strcat( cStatement, "', 'APP STOP', getdate(), '" );
		strcat( cStatement, m_cUsername );
		strcat( cStatement, "' )" );
		sys_update_log( 0, LOG_SQL, cStatement );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		if( nRetval != SQL_SUCCESS && nRetval != SQL_SUCCESS_WITH_INFO )
			sys_update_log( 0, LOG_ERR, cStatement );
		nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
		nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
	}
	return TRUE;
}

int insert_broadcast_detail(int devh, int code)
{
	int devh1 = devh/2;
	
	SQLHSTMT hstmt;
	int nRetval;
	char cStatement[700];
	
	if( hdbc == NULL )
		return FALSE;

	if( oPort[devh1].switched == ALARM ){

		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return FALSE;
		}

		if( oPort[devh1].connect_dttm == 0 )
			sprintf( cStatement, "insert into broadcastdetail_ivr(alarm_id,contact_no,contact_mode,call_mode,call_str_dttm,call_end_dttm,call_result,duration,isdn_no,channel_no,update_dttm) values (%d, '%s', 'H', 'A', dateadd(s,-%d,getdate()), getdate(), %d, %d, %d, %d, getdate())",
				oPort[devh1].nAlarmid, oPort[devh1].cContactno, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].nCallresult, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].nBoard, oPort[devh1].nChannel );
		else
			sprintf( cStatement, "insert into broadcastdetail_ivr(alarm_id,contact_no,contact_mode,call_mode,call_str_dttm,call_con_dttm,call_end_dttm,call_result,duration,isdn_no,channel_no,update_dttm) values (%d, '%s', 'H', 'A', dateadd(s,-%d,getdate()), dateadd(s,-%d,getdate()), getdate(), %d, %d, %d, %d, getdate())",
				oPort[devh1].nAlarmid, oPort[devh1].cContactno, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].end_dttm - oPort[devh1].connect_dttm, oPort[devh1].nCallresult, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].nBoard, oPort[devh1].nChannel );	
		sys_update_log( 0, LOG_SQL, cStatement );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		if( nRetval != SQL_SUCCESS && nRetval != SQL_SUCCESS_WITH_INFO )
			sys_update_log( 0, LOG_ERR, cStatement );
		nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
		nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

	}

	if( oPort[devh1].switched == OUTGOING ){
		
		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return FALSE;
		}

		if( oPort[devh1].connect_dttm == 0 ){
			//if( oPort[devh1].nMobid > 0  && oPort[devh1].cExcuseflg[0] == 'N' && oPort[devh1].cAckstatus != 'S' ){
			if( oPort[devh1].nMobid > 0  && oPort[devh1].cAckstatus != 'S' ){
				sprintf( cStatement, "insert into broadcastdetail_ivr(mob_id,adhoc_id,nric,pin,whichcontactno,contact_no,contact_mode,whichbroadcast,call_mode,call_str_dttm,call_end_dttm,call_digits,call_result,duration,ack_status,isdn_no,channel_no,update_dttm,sms_status) values (%d, %d, '%s', '%s', %d, '%s', '%c', %d, 'O', dateadd(s,-%d,getdate()), getdate(), '%s', %d, %d, '%c', %d, %d, getdate(),'P')",
					oPort[devh1].nMobid, oPort[devh1].nAdhocid, oPort[devh1].cNRIC, oPort[devh1].cPin, oPort[devh1].nWhichcontactno, oPort[devh1].cContactno, oPort[devh1].cCalltype, oPort[devh1].nWhichbroadcast, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].cCalldigits, oPort[devh1].nCallresult, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].cAckstatus, oPort[devh1].nBoard, oPort[devh1].nChannel );
			}
			else{
				sprintf( cStatement, "insert into broadcastdetail_ivr(mob_id,adhoc_id,nric,pin,whichcontactno,contact_no,contact_mode,whichbroadcast,call_mode,call_str_dttm,call_end_dttm,call_digits,call_result,duration,ack_status,isdn_no,channel_no,update_dttm) values (%d, %d, '%s', '%s', %d, '%s', '%c', %d, 'O', dateadd(s,-%d,getdate()), getdate(), '%s', %d, %d, '%c', %d, %d, getdate())",
					oPort[devh1].nMobid, oPort[devh1].nAdhocid, oPort[devh1].cNRIC, oPort[devh1].cPin, oPort[devh1].nWhichcontactno, oPort[devh1].cContactno, oPort[devh1].cCalltype, oPort[devh1].nWhichbroadcast, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].cCalldigits, oPort[devh1].nCallresult, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].cAckstatus, oPort[devh1].nBoard, oPort[devh1].nChannel );
			}
		}
		else{
			//if( oPort[devh1].nMobid > 0 && oPort[devh1].cExcuseflg[0] == 'N' && oPort[devh1].cAckstatus != 'S' ){
			if( oPort[devh1].nMobid > 0 && oPort[devh1].cAckstatus != 'S' ){
				sprintf( cStatement, "insert into broadcastdetail_ivr(mob_id,adhoc_id,nric,pin,whichcontactno,contact_no,contact_mode,whichbroadcast,call_mode,call_str_dttm,call_con_dttm,call_end_dttm,call_digits,call_result,duration,ack_status,isdn_no,channel_no,update_dttm,sms_status) values (%d, %d, '%s', '%s', %d, '%s', '%c', %d, 'O', dateadd(s,-%d,getdate()), dateadd(s,-%d,getdate()), getdate(), '%s', %d, %d, '%c', %d, %d, getdate(),'P')",
					oPort[devh1].nMobid, oPort[devh1].nAdhocid, oPort[devh1].cNRIC, oPort[devh1].cPin, oPort[devh1].nWhichcontactno, oPort[devh1].cContactno, oPort[devh1].cCalltype, oPort[devh1].nWhichbroadcast, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].end_dttm - oPort[devh1].connect_dttm, oPort[devh1].cCalldigits, oPort[devh1].nCallresult, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].cAckstatus, oPort[devh1].nBoard, oPort[devh1].nChannel );
			}
			else{
				sprintf( cStatement, "insert into broadcastdetail_ivr(mob_id,adhoc_id,nric,pin,whichcontactno,contact_no,contact_mode,whichbroadcast,call_mode,call_str_dttm,call_con_dttm,call_end_dttm,call_digits,call_result,duration,ack_status,isdn_no,channel_no,update_dttm) values (%d, %d, '%s', '%s', %d, '%s', '%c', %d, 'O', dateadd(s,-%d,getdate()), dateadd(s,-%d,getdate()), getdate(), '%s', %d, %d, '%c', %d, %d, getdate())",
					oPort[devh1].nMobid, oPort[devh1].nAdhocid, oPort[devh1].cNRIC, oPort[devh1].cPin, oPort[devh1].nWhichcontactno, oPort[devh1].cContactno, oPort[devh1].cCalltype, oPort[devh1].nWhichbroadcast, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].end_dttm - oPort[devh1].connect_dttm, oPort[devh1].cCalldigits, oPort[devh1].nCallresult, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].cAckstatus, oPort[devh1].nBoard, oPort[devh1].nChannel );
			}
		}
		sys_update_log( 0, LOG_SQL, cStatement );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		if( nRetval != SQL_SUCCESS && nRetval != SQL_SUCCESS_WITH_INFO )
			sys_update_log( 0, LOG_ERR, cStatement );
		nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
		nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
	
	}
	
	if( oPort[devh1].switched == INCOMING ){

		if( oPort[devh1].nMobid == 0 ){
			chkmobid( devh1 );
		}
		
		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return FALSE;
		}

		if( oPort[devh1].connect_dttm == 0 ){
			//if( oPort[devh1].nMobid > 0 && oPort[devh1].cExcuseflg[0] == 'N' && oPort[devh1].cAckstatus != 'S' ){
			if( oPort[devh1].nMobid > 0 && oPort[devh1].cAckstatus != 'S' ){
				sprintf( cStatement, "insert into broadcastdetail_ivr(mob_id,adhoc_id,nric,pin,whichcontactno,contact_no,contact_mode,whichbroadcast,call_mode,call_str_dttm,call_end_dttm,call_digits,call_result,duration,ack_status,isdn_no,channel_no,update_dttm,sms_status) values (%d, %d, '%s', '%s', %d, '%s', '%c', %d, 'I', dateadd(s,-%d,getdate()), getdate(), '%s', %d, %d, '%c',%d, %d, getdate(),'P')",
					oPort[devh1].nMobid, oPort[devh1].nAdhocid, oPort[devh1].cNRIC, oPort[devh1].cPin, oPort[devh1].nWhichcontactno, oPort[devh1].cContactno, oPort[devh1].cCalltype, oPort[devh1].nWhichbroadcast, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].cCalldigits, oPort[devh1].nCallresult, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].cAckstatus, oPort[devh1].nBoard, oPort[devh1].nChannel );
			}
			else{
				sprintf( cStatement, "insert into broadcastdetail_ivr(mob_id,adhoc_id,nric,pin,whichcontactno,contact_no,contact_mode,whichbroadcast,call_mode,call_str_dttm,call_end_dttm,call_digits,call_result,duration,ack_status,isdn_no,channel_no,update_dttm) values (%d, %d, '%s', '%s', %d, '%s', '%c', %d, 'I', dateadd(s,-%d,getdate()), getdate(), '%s', %d, %d, '%c',%d, %d, getdate())",
					oPort[devh1].nMobid, oPort[devh1].nAdhocid, oPort[devh1].cNRIC, oPort[devh1].cPin, oPort[devh1].nWhichcontactno, oPort[devh1].cContactno, oPort[devh1].cCalltype, oPort[devh1].nWhichbroadcast, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].cCalldigits, oPort[devh1].nCallresult, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].cAckstatus, oPort[devh1].nBoard, oPort[devh1].nChannel );
			}
		}
		else{
			//if( oPort[devh1].nMobid > 0 && oPort[devh1].cExcuseflg[0] == 'N' && oPort[devh1].cAckstatus != 'S' ){
			if( oPort[devh1].nMobid > 0 && oPort[devh1].cAckstatus != 'S' ){
				sprintf( cStatement, "insert into broadcastdetail_ivr(mob_id,adhoc_id,nric,pin,whichcontactno,contact_no,contact_mode,whichbroadcast,call_mode,call_str_dttm,call_con_dttm,call_end_dttm,call_digits,call_result,duration,ack_status,isdn_no,channel_no,update_dttm,sms_status) values (%d, %d, '%s', '%s', %d, '%s', '%c', %d, 'I', dateadd(s,-%d,getdate()), dateadd(s,-%d,getdate()), getdate(), '%s', %d, %d, '%c', %d, %d, getdate(),'P')",
					oPort[devh1].nMobid, oPort[devh1].nAdhocid, oPort[devh1].cNRIC, oPort[devh1].cPin, oPort[devh1].nWhichcontactno, oPort[devh1].cContactno, oPort[devh1].cCalltype, oPort[devh1].nWhichbroadcast, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].end_dttm - oPort[devh1].connect_dttm, oPort[devh1].cCalldigits, oPort[devh1].nCallresult, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].cAckstatus, oPort[devh1].nBoard, oPort[devh1].nChannel );
			}
			else{
				sprintf( cStatement, "insert into broadcastdetail_ivr(mob_id,adhoc_id,nric,pin,whichcontactno,contact_no,contact_mode,whichbroadcast,call_mode,call_str_dttm,call_con_dttm,call_end_dttm,call_digits,call_result,duration,ack_status,isdn_no,channel_no,update_dttm) values (%d, %d, '%s', '%s', %d, '%s', '%c', %d, 'I', dateadd(s,-%d,getdate()), dateadd(s,-%d,getdate()), getdate(), '%s', %d, %d, '%c', %d, %d, getdate())",
					oPort[devh1].nMobid, oPort[devh1].nAdhocid, oPort[devh1].cNRIC, oPort[devh1].cPin, oPort[devh1].nWhichcontactno, oPort[devh1].cContactno, oPort[devh1].cCalltype, oPort[devh1].nWhichbroadcast, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].end_dttm - oPort[devh1].connect_dttm, oPort[devh1].cCalldigits, oPort[devh1].nCallresult, oPort[devh1].end_dttm - oPort[devh1].start_dttm, oPort[devh1].cAckstatus, oPort[devh1].nBoard, oPort[devh1].nChannel );
			}
		}
		sys_update_log( 0, LOG_SQL, cStatement );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		if( nRetval != SQL_SUCCESS && nRetval != SQL_SUCCESS_WITH_INFO )
			sys_update_log( 0, LOG_ERR, cStatement );
		nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
		nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
	
	}

	return TRUE;
}

void sys_update_log(int nChannel, int nMode, char * szMsg)
{
	FILE *pFile;
    char cFilename[25];
	char cDate[25], cTime[25];
	
	if( ENABLE_LOG == 0 )
		return;

	_strdate( cDate );
	cDate[8] = '\0';
	cDate[2] = '-';
	cDate[5] = '-';
	sprintf( cFilename, "log%s.txt", cDate );
	_strtime( cTime );

	if( nMode == LOG_INF && ENABLE_LOG_INF == 1 ){
		pFile = fopen( cFilename, "a" );
		fprintf( pFile,"%s - Channel %d INF %s\n", cTime, nChannel, szMsg );
		fclose( pFile );
	}

	if( nMode == LOG_SQL && ENABLE_LOG_SQL == 1 ){
		pFile = fopen( cFilename, "a" );
		fprintf( pFile,"%s - Channel %d SQL %s\n", cTime, nChannel, szMsg );
		fclose( pFile );
	}

	if( nMode == LOG_EVT && ENABLE_LOG_EVT == 1 ){
		pFile = fopen( cFilename, "a" );
		fprintf( pFile,"%s - Channel %d EVT %s\n", cTime, nChannel, szMsg );
		fclose( pFile );
	}

	if( nMode == LOG_ERR && ENABLE_LOG_ERR == 1 ){
		pFile = fopen( cFilename, "a" );
		fprintf( pFile,"%s - Channel %d ERR %s\n", cTime, nChannel, szMsg );
		fclose( pFile );
	}

}

void check_alarm_log()
{
	SQLHSTMT hstmt;
	int nRetval;
	char cStatement[270];
	int i, nRecordempty;
	SQLINTEGER nAlarmid = 0,cb_nAlarmid = 0;
	SQLCHAR cAlarmcode[11], cAlarmno[9];
	SQLINTEGER cb_cAlarmcode = SQL_NTS, cb_cAlarmno = SQL_NTS;

	if( hdbc == NULL )
		return;

	nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
	if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
		hdbc = NULL;
		return;
	}

	strcpy( cStatement, "select alarm_id,alarm_code,alarm_no from alarm_log (nolock) where alarm_result = 'P' and alarm_type = 'P'");
	sys_update_log( 0, LOG_SQL, cStatement );
	nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
	nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nAlarmid,0,&cb_nAlarmid);
	nRetval = SQLBindCol(hstmt,2,SQL_C_CHAR,&cAlarmcode,11,&cb_cAlarmcode);
	nRetval = SQLBindCol(hstmt,3,SQL_C_CHAR,&cAlarmno,9,&cb_cAlarmno);
	nRetval = SQLFetch(hstmt);
	if(  nRetval == SQL_SUCCESS || nRetval == SQL_SUCCESS_WITH_INFO ){
		nRecordempty = FALSE; 
	}
	else{
		nRecordempty = TRUE;
	}
	nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
	nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

	if( nRecordempty == TRUE )
		return;
	
	for( i = m_total_chans; i >= 1; i-- ){
		if( oPort[i].flag == 0 && oPort[i].blocked == NO ){
			time( &oPort[i].start_dttm );
			oPort[i].connect_dttm = 0;
			time( &oPort[i].end_dttm );
			
			nRetval = gc_SetCallingNum(oPort[i].ldev, "63099052");

			nRetval = gc_MakeCall(oPort[i].ldev, &oPort[i].crn, cAlarmno, NULL, 60, EV_ASYNC);
			if( nRetval < 0 ){
				sprintf( cStatement, "gc_MakeCall( %d, %d, %s, NULL 60, EV_ASYNS ) return %d",
					oPort[i].ldev, oPort[i].crn, cAlarmno, nRetval );
				sys_update_log( i, LOG_ERR, cStatement );
				return;
			}
			
			oPort[i].flag = 1;
			oPort[i].state = ST_MACKCALL;
			oPort[i].switched = ALARM;
			oPort[i].in_process = 0;
			oPort[i].nAlarmid = nAlarmid;
			strcpy(oPort[i].cContactno, cAlarmno);
			strcpy(oPort[i].cAlarmcode,cAlarmcode);
			
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}
			sprintf( cStatement, "update alarm_log set alarm_result = 'D', proc_dttm = getdate() where alarm_id = %d",
				nAlarmid );
			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
			break;
		}
	} // END FOR
}

int check_broadcast_timekeeper()
{
	SQLHSTMT hstmt;
	int nRetval;
	int nRecordempty = FALSE;
	char cStatement[100];
	SQLINTEGER nMobid = 0,cb_nMobid = 0;
	
	if( hdbc == NULL )
		return FALSE;

	nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
	if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
		hdbc = NULL;
		return FALSE;
	}

	strcpy( cStatement, "select mob_id from mobbroadcast_timekeeper (nolock) where broadcast_complete_i = 'D'");
	nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
	nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nMobid,0,&cb_nMobid);
	nRetval = SQLFetch(hstmt);
	if( nRetval == SQL_SUCCESS || nRetval == SQL_SUCCESS_WITH_INFO ){
		nRecordempty = FALSE; 
	}
	else{
		nRecordempty = TRUE;
	}
	nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
	nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

	if( nRecordempty == TRUE )
		return FALSE;
	else
		return TRUE;
}

int check_broadcast_timetable(char *cContactno)
{
	SQLHSTMT hstmt;
	int nRetval;
	int nRecordempty = FALSE;
	char cStatement[170];
	SQLINTEGER nMobid = 0,cb_nMobid = 0;
	SQLINTEGER nGroupid = 0,cb_nGroupid = 0;

	if( hdbc == NULL )
		return FALSE;

	nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
	if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
		hdbc = NULL;
		return FALSE;
	}

	strcpy( cStatement, "select mob_id from mobbroadcast_timetable (nolock) where getdate() between mob_str_dttm and mob_end_dttm and status != 'C'");
	nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
	nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nMobid,0,&cb_nMobid);
	nRetval = SQLFetch(hstmt);
	if( nRetval == SQL_SUCCESS || nRetval == SQL_SUCCESS_WITH_INFO ){
		nRecordempty = FALSE; 
	}
	else{
		nRecordempty = TRUE;
	}
	nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
	nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

	if( nRecordempty == FALSE )
		return TRUE;
	

	nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
	if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
		hdbc = NULL;
		return FALSE;
	}
	strcpy( cStatement, "select group_id from alarm_group (nolock) where group_type = 'M'");
	nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
	nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nGroupid,0,&cb_nGroupid);
	nRetval = SQLFetch(hstmt);
	if( nRetval == SQL_SUCCESS || nRetval == SQL_SUCCESS_WITH_INFO ){
		nRecordempty = FALSE; 
	}
	else{
		nRecordempty = TRUE;
	}
	nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
	nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
	if( nRecordempty == TRUE )
		return FALSE;
	
	nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
	if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
		hdbc = NULL;
		return FALSE;
	}
	sprintf( cStatement, "select group_id from alarm_no (nolock) where group_id = %d and alarm_no = '%s'",
		nGroupid, cContactno );
	nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
	nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nGroupid,0,&cb_nGroupid);
	nRetval = SQLFetch(hstmt);
	if( nRetval == SQL_SUCCESS || nRetval == SQL_SUCCESS_WITH_INFO ){
		nRecordempty = FALSE; 
	}
	else{
		nRecordempty = TRUE;
	}
	nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
	nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
	if( nRecordempty == TRUE )
		return FALSE;
	
	return TRUE;
}


int check_ic_type(int devh, char *cContactno)
{
	SQLHSTMT hstmt;
	int nRetval;
	char cStatement[270];
	SQLINTEGER nICtype = 0,cb_nICtype = 0;
	int devh1 = devh/2;
	
	if( hdbc == NULL )
		return FALSE;
	
	if( oPort[devh1].cContactno[0] == ' ' || oPort[devh1].cContactno[0] == '\0' ){
		oPort[devh1].nICtype = 0;	
		return TRUE;
	}
	
	nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
	if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
		hdbc = NULL;
		return FALSE;
	}
	
	sprintf(cStatement,"select ic from mobnominalroll (nolock) where cn1st = '%s' or cn2nd = '%s' or cn3rd = '%s' or cn4th = '%s' or cn5th = '%s' or cn6th = '%s' order by mob_id",
		oPort[devh1].cContactno, oPort[devh1].cContactno, oPort[devh1].cContactno, 
		oPort[devh1].cContactno, oPort[devh1].cContactno, oPort[devh1].cContactno );
	
	nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
	nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nICtype,0,&cb_nICtype);
	nRetval = SQLFetch(hstmt);
	if( nRetval == SQL_SUCCESS || nRetval == SQL_SUCCESS_WITH_INFO ){
		oPort[devh1].nICtype = nICtype;	 
	}
	else{
		oPort[devh1].nICtype = 0;	
	}
	nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
	nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

	return TRUE;
}


void check_mobnominalroll()
{
	SQLHSTMT hstmt;
	int nRetval;
	char cStatement[550];
	int i, nRecordempty;
	int nWhichcontactno;
	
	SQLINTEGER nMobid = 0,cb_nMobid = 0;	
	SQLINTEGER nWhichbroadcast = 0,cb_nWhichbroadcast = 0;
	SQLINTEGER nPriority = 0,cb_nPriority = 0;
	SQLCHAR cNRIC[21], cPin[21];
	SQLINTEGER cb_cNRIC = SQL_NTS, cb_cPin = SQL_NTS;
	SQLCHAR cContactno[9], cCalltype[2];
	SQLINTEGER cb_cContactno = SQL_NTS, cb_cCalltype = SQL_NTS;
	SQLCHAR cBrofile1[10], cBrofile2[10];
	SQLINTEGER cb_cBrofile1 = SQL_NTS, cb_cBrofile2 = SQL_NTS;
	SQLCHAR cRegistercode[5];
	SQLINTEGER cb_cRegistercode = SQL_NTS;
	SQLINTEGER nICtype = 0,cb_nICtype = 0;

	if( hdbc == NULL )
		return;

//	m_total_outgoing = 1;
	for( i = m_total_chans; i > m_total_chans - m_total_outgoing; i-- ){
		if( oPort[i].flag == 0 && oPort[i].blocked == NO ){
			break;
		}
	}
	if( i <= m_total_chans - m_total_outgoing )
		return;

//	if( m_outgoing_chans < m_total_outgoing ){
		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return;
		}
		strcpy( cStatement, "select top 1 mob_id,whichbroadcast,nric,pin,priority,registercode,cn1st,cn1st_mode,b_voxfile1,b_voxfile2,ic ");
		strcat( cStatement, "from mobnominalroll (nolock) where ack_status = 'N' and cn1st_status = 'F' and cn1st != '' and cn1st_mode != '2' and (e2_dttm < getdate() - 0.03125 or e2_dttm is null) and excuse_flag = 'N'");
		
		sys_update_log( 0, LOG_SQL, cStatement );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nMobid,0,&cb_nMobid);
		nRetval = SQLBindCol(hstmt,2,SQL_C_SLONG,&nWhichbroadcast,0,&cb_nWhichbroadcast);
		nRetval = SQLBindCol(hstmt,3,SQL_C_CHAR,&cNRIC,21,&cb_cNRIC);
		nRetval = SQLBindCol(hstmt,4,SQL_C_CHAR,&cPin,21,&cb_cPin);
		nRetval = SQLBindCol(hstmt,5,SQL_C_SLONG,&nPriority,0,&cb_nPriority);
		nRetval = SQLBindCol(hstmt,6,SQL_C_CHAR,&cRegistercode,5,&cb_cRegistercode);
		nRetval = SQLBindCol(hstmt,7,SQL_C_CHAR,&cContactno,9,&cb_cContactno);
		nRetval = SQLBindCol(hstmt,8,SQL_C_CHAR,&cCalltype,2,&cb_cCalltype);
		nRetval = SQLBindCol(hstmt,9,SQL_C_CHAR,&cBrofile1,10,&cb_cBrofile1);
		nRetval = SQLBindCol(hstmt,10,SQL_C_CHAR,&cBrofile2,10,&cb_cBrofile2);
		nRetval = SQLBindCol(hstmt,11,SQL_C_SLONG,&nICtype,0,&cb_nICtype);
		nRetval = SQLFetch(hstmt);
		if( nRetval == SQL_SUCCESS || nRetval == SQL_SUCCESS_WITH_INFO ){
			nRecordempty = FALSE;
			nWhichcontactno = 1;
		}
		else{
			nRecordempty = TRUE;
		}
		nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
		nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

		if( nRecordempty == TRUE ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			strcpy( cStatement, "select top 1 mob_id,whichbroadcast,nric,pin,priority,registercode,cn2nd,cn2nd_mode,b_voxfile1,b_voxfile2,ic ");
			strcat( cStatement, "from mobnominalroll (nolock) where ack_status = 'N' and cn2nd_status = 'F' and cn2nd != '' and cn2nd_mode != '2' and (e2_dttm < getdate() - 0.03125 or e2_dttm is null) and excuse_flag = 'N'");
			
			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nMobid,0,&cb_nMobid);
			nRetval = SQLBindCol(hstmt,2,SQL_C_SLONG,&nWhichbroadcast,0,&cb_nWhichbroadcast);
			nRetval = SQLBindCol(hstmt,3,SQL_C_CHAR,&cNRIC,21,&cb_cNRIC);
			nRetval = SQLBindCol(hstmt,4,SQL_C_CHAR,&cPin,21,&cb_cPin);
			nRetval = SQLBindCol(hstmt,5,SQL_C_SLONG,&nPriority,0,&cb_nPriority);
			nRetval = SQLBindCol(hstmt,6,SQL_C_CHAR,&cRegistercode,5,&cb_cRegistercode);
			nRetval = SQLBindCol(hstmt,7,SQL_C_CHAR,&cContactno,9,&cb_cContactno);
			nRetval = SQLBindCol(hstmt,8,SQL_C_CHAR,&cCalltype,2,&cb_cCalltype);
			nRetval = SQLBindCol(hstmt,9,SQL_C_CHAR,&cBrofile1,10,&cb_cBrofile1);
			nRetval = SQLBindCol(hstmt,10,SQL_C_CHAR,&cBrofile2,10,&cb_cBrofile2);
			nRetval = SQLBindCol(hstmt,11,SQL_C_SLONG,&nICtype,0,&cb_nICtype);
			nRetval = SQLFetch(hstmt);
			if( nRetval == SQL_SUCCESS || nRetval == SQL_SUCCESS_WITH_INFO ){
				nRecordempty = FALSE;
				nWhichcontactno = 2;
			}
			else{
				nRecordempty = TRUE;
			}
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}

		if( nRecordempty == TRUE ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			strcpy( cStatement, "select top 1 mob_id,whichbroadcast,nric,pin,priority,registercode,cn3rd,cn3rd_mode,b_voxfile1,b_voxfile2,ic ");
			strcat( cStatement, "from mobnominalroll (nolock) where ack_status = 'N' and cn3rd_status = 'F' and cn3rd != '' and cn3rd_mode != '2' and (e2_dttm < getdate() - 0.03125 or e2_dttm is null) and excuse_flag = 'N'");
			
			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nMobid,0,&cb_nMobid);
			nRetval = SQLBindCol(hstmt,2,SQL_C_SLONG,&nWhichbroadcast,0,&cb_nWhichbroadcast);
			nRetval = SQLBindCol(hstmt,3,SQL_C_CHAR,&cNRIC,21,&cb_cNRIC);
			nRetval = SQLBindCol(hstmt,4,SQL_C_CHAR,&cPin,21,&cb_cPin);
			nRetval = SQLBindCol(hstmt,5,SQL_C_SLONG,&nPriority,0,&cb_nPriority);
			nRetval = SQLBindCol(hstmt,6,SQL_C_CHAR,&cRegistercode,5,&cb_cRegistercode);
			nRetval = SQLBindCol(hstmt,7,SQL_C_CHAR,&cContactno,9,&cb_cContactno);
			nRetval = SQLBindCol(hstmt,8,SQL_C_CHAR,&cCalltype,2,&cb_cCalltype);
			nRetval = SQLBindCol(hstmt,9,SQL_C_CHAR,&cBrofile1,10,&cb_cBrofile1);
			nRetval = SQLBindCol(hstmt,10,SQL_C_CHAR,&cBrofile2,10,&cb_cBrofile2);
			nRetval = SQLBindCol(hstmt,11,SQL_C_SLONG,&nICtype,0,&cb_nICtype);
			nRetval = SQLFetch(hstmt);
			if( nRetval == SQL_SUCCESS || nRetval == SQL_SUCCESS_WITH_INFO ){
				nRecordempty = FALSE;
				nWhichcontactno = 3;
			}
			else{
				nRecordempty = TRUE;
			}
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}

		if( nRecordempty == TRUE ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			strcpy( cStatement, "select top 1 mob_id,whichbroadcast,nric,pin,priority,registercode,cn4th,cn4th_mode,b_voxfile1,b_voxfile2,ic ");
			strcat( cStatement, "from mobnominalroll (nolock) where ack_status = 'N' and cn4th_status = 'F' and cn4th != '' and cn4th_mode != '2' and (e2_dttm < getdate() - 0.03125 or e2_dttm is null) and excuse_flag = 'N'");
			
			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nMobid,0,&cb_nMobid);
			nRetval = SQLBindCol(hstmt,2,SQL_C_SLONG,&nWhichbroadcast,0,&cb_nWhichbroadcast);
			nRetval = SQLBindCol(hstmt,3,SQL_C_CHAR,&cNRIC,21,&cb_cNRIC);
			nRetval = SQLBindCol(hstmt,4,SQL_C_CHAR,&cPin,21,&cb_cPin);
			nRetval = SQLBindCol(hstmt,5,SQL_C_SLONG,&nPriority,0,&cb_nPriority);
			nRetval = SQLBindCol(hstmt,6,SQL_C_CHAR,&cRegistercode,5,&cb_cRegistercode);
			nRetval = SQLBindCol(hstmt,7,SQL_C_CHAR,&cContactno,9,&cb_cContactno);
			nRetval = SQLBindCol(hstmt,8,SQL_C_CHAR,&cCalltype,2,&cb_cCalltype);
			nRetval = SQLBindCol(hstmt,9,SQL_C_CHAR,&cBrofile1,10,&cb_cBrofile1);
			nRetval = SQLBindCol(hstmt,10,SQL_C_CHAR,&cBrofile2,10,&cb_cBrofile2);
			nRetval = SQLBindCol(hstmt,11,SQL_C_SLONG,&nICtype,0,&cb_nICtype);
			nRetval = SQLFetch(hstmt);
			if( nRetval == SQL_SUCCESS || nRetval == SQL_SUCCESS_WITH_INFO ){
				nRecordempty = FALSE;
				nWhichcontactno = 4;
			}
			else{
				nRecordempty = TRUE;
			}
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}

		if( nRecordempty == TRUE ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			strcpy( cStatement, "select top 1 mob_id,whichbroadcast,nric,pin,priority,registercode,cn5th,cn5th_mode,b_voxfile1,b_voxfile2,ic ");
			strcat( cStatement, "from mobnominalroll (nolock) where ack_status = 'N' and cn5th_status = 'F' and cn5th != '' and cn5th_mode != '2' and (e2_dttm < getdate() - 0.03125 or e2_dttm is null) and excuse_flag = 'N'");
			
			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nMobid,0,&cb_nMobid);
			nRetval = SQLBindCol(hstmt,2,SQL_C_SLONG,&nWhichbroadcast,0,&cb_nWhichbroadcast);
			nRetval = SQLBindCol(hstmt,3,SQL_C_CHAR,&cNRIC,21,&cb_cNRIC);
			nRetval = SQLBindCol(hstmt,4,SQL_C_CHAR,&cPin,21,&cb_cPin);
			nRetval = SQLBindCol(hstmt,5,SQL_C_SLONG,&nPriority,0,&cb_nPriority);
			nRetval = SQLBindCol(hstmt,6,SQL_C_CHAR,&cRegistercode,5,&cb_cRegistercode);
			nRetval = SQLBindCol(hstmt,7,SQL_C_CHAR,&cContactno,9,&cb_cContactno);
			nRetval = SQLBindCol(hstmt,8,SQL_C_CHAR,&cCalltype,2,&cb_cCalltype);
			nRetval = SQLBindCol(hstmt,9,SQL_C_CHAR,&cBrofile1,10,&cb_cBrofile1);
			nRetval = SQLBindCol(hstmt,10,SQL_C_CHAR,&cBrofile2,10,&cb_cBrofile2);
			nRetval = SQLBindCol(hstmt,11,SQL_C_SLONG,&nICtype,0,&cb_nICtype);
			nRetval = SQLFetch(hstmt);
			if( nRetval == SQL_SUCCESS || nRetval == SQL_SUCCESS_WITH_INFO ){
				nRecordempty = FALSE;
				nWhichcontactno = 5;
			}
			else{
				nRecordempty = TRUE;
			}
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}

		if( nRecordempty == TRUE ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			strcpy( cStatement, "select top 1 mob_id,whichbroadcast,nric,pin,priority,registercode,cn6th,cn6th_mode,b_voxfile1,b_voxfile2,ic ");
			strcat( cStatement, "from mobnominalroll (nolock) where ack_status = 'N' and cn6th_status = 'F' and cn6th != '' and cn6th_mode != '2' and (e2_dttm < getdate() - 0.03125 or e2_dttm is null) and excuse_flag = 'N'");
			
			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nMobid,0,&cb_nMobid);
			nRetval = SQLBindCol(hstmt,2,SQL_C_SLONG,&nWhichbroadcast,0,&cb_nWhichbroadcast);
			nRetval = SQLBindCol(hstmt,3,SQL_C_CHAR,&cNRIC,21,&cb_cNRIC);
			nRetval = SQLBindCol(hstmt,4,SQL_C_CHAR,&cPin,21,&cb_cPin);
			nRetval = SQLBindCol(hstmt,5,SQL_C_SLONG,&nPriority,0,&cb_nPriority);
			nRetval = SQLBindCol(hstmt,6,SQL_C_CHAR,&cRegistercode,5,&cb_cRegistercode);
			nRetval = SQLBindCol(hstmt,7,SQL_C_CHAR,&cContactno,9,&cb_cContactno);
			nRetval = SQLBindCol(hstmt,8,SQL_C_CHAR,&cCalltype,2,&cb_cCalltype);
			nRetval = SQLBindCol(hstmt,9,SQL_C_CHAR,&cBrofile1,10,&cb_cBrofile1);
			nRetval = SQLBindCol(hstmt,10,SQL_C_CHAR,&cBrofile2,10,&cb_cBrofile2);
			nRetval = SQLBindCol(hstmt,11,SQL_C_SLONG,&nICtype,0,&cb_nICtype);
			nRetval = SQLFetch(hstmt);
			if( nRetval == SQL_SUCCESS || nRetval == SQL_SUCCESS_WITH_INFO ){
				nRecordempty = FALSE;
				nWhichcontactno = 6;
			}
			else{
				nRecordempty = TRUE;
			}
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}

		if( nRecordempty == FALSE ){
			for( i = m_total_chans; i >= 1; i-- ){
				if( oPort[i].flag == 0 && oPort[i].blocked == NO ){
					cNRIC[20] = '\0';
					cPin[20] = '\0';
								
					update_mobnominalroll( nMobid, nWhichcontactno, cPin, 'B', 0 );
					time( &oPort[i].start_dttm );
					oPort[i].connect_dttm = 0;
					time( &oPort[i].end_dttm );
					
					nRetval = gc_MakeCall(oPort[i].ldev, &oPort[i].crn, cContactno, NULL, 60, EV_ASYNC);
					if( nRetval != EGC_NOERR ){
						sprintf( cStatement, "gc_MakeCall( %d, %d, %s, NULL 60, EV_ASYNS ) return %d",
							oPort[i].ldev, oPort[i].crn, cContactno, nRetval );
						sys_update_log( i, LOG_ERR, cStatement );
					}
					m_outgoing_chans = m_outgoing_chans + 1;

					oPort[i].flag = 1;
					oPort[i].state = ST_MACKCALL;
					oPort[i].switched = OUTGOING;
					oPort[i].in_process = 0;

					oPort[i].nMobid = nMobid;
					oPort[i].nWhichcontactno = nWhichcontactno;
					oPort[i].nWhichbroadcast = nWhichbroadcast;
					strcpy(oPort[i].cRegistercode, cRegistercode ); 
					oPort[i].nPriority = nPriority;
					strcpy(oPort[i].cNRIC, cNRIC);
					strcpy(oPort[i].cPin, cPin);
					strcpy(oPort[i].cContactno, cContactno);
					oPort[i].cCalltype = cCalltype[0];
					strcpy(oPort[i].cBrofile1, cBrofile1);
					strcpy(oPort[i].cBrofile2, cBrofile2);
					strcpy(oPort[i].cAckfile1, "" );
					strcpy(oPort[i].cAckfile2, "" );
					strcpy(oPort[i].cCalldigits, "" );
					oPort[i].cAckstatus = 'N';
					oPort[i].nCallresult = 0;
					oPort[i].cBroreset = 'N';
					oPort[i].noofretry = 0;
					oPort[i].noofreplay = 0;
					oPort[i].noofpin = 0;
					oPort[i].noofmobileno = 0;
					strcpy(oPort[i].cAckMobileno, "" );
					strcpy(oPort[i].playnumber, "" );
					oPort[i].playno = 0;
					oPort[i].nLanguage = 0;
					oPort[i].nICtype = nICtype;
					oPort[i].nAdhocid = 0;
			
					break;
				}
			}
		} // nRecordempty == FALSE (mobnominalroll)
		else{
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}

			strcpy( cStatement, "update mobbroadcast_timekeeper set broadcast_complete_i = 'C' where broadcast_complete_i = 'D'" );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
//	} // m_outgoing_chans < m_total_outgoing
}

void update_alarm_log(int nAlarmid, char cStatus)
{
	SQLHSTMT hstmt;
	int nRetval;
	char cStatement[270];
	
	if( hdbc == NULL )
		return;

	nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
	if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
		hdbc = NULL;
		return;
	}

	sprintf( cStatement, "update alarm_log set alarm_result = '%c', end_dttm = getdate() where alarm_id = %d",
		cStatus, nAlarmid );
	nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
	nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
	nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

}

void chkmobid(int devh)
{
	SQLHSTMT hstmt;
	int nRetval;
	char cStatement[320];
	SQLINTEGER nMobid = 0,cb_nMobid = 0;	
	
	if( hdbc == NULL ){
		oPort[devh].nMobid = 0;
		return;
	}

	nRetval = SQLAllocHandle( SQL_HANDLE_STMT, hdbc,&hstmt );
	if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
		hdbc = NULL;
		oPort[devh].nMobid = 0;
		return;
	}

	sprintf(cStatement,"select mob_id from mobnominalroll (nolock) where cn1st = '%s' or cn2nd = '%s' or cn3rd = '%s' or cn4th = '%s' or cn5th = '%s' or cn6th = '%s' order by mob_id",
		oPort[devh].cContactno, oPort[devh].cContactno, oPort[devh].cContactno, 
		oPort[devh].cContactno, oPort[devh].cContactno, oPort[devh].cContactno );
	
	sys_update_log( 0, LOG_SQL, cStatement );
	nRetval = SQLExecDirect(hstmt,(unsigned char *)&cStatement,SQL_NTS);
	nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nMobid,sizeof(SQLINTEGER),&cb_nMobid);
	nRetval = SQLFetch( hstmt );
	SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
	
	oPort[devh].nMobid = nMobid;
}


void getservice(int devh)
{
	char tmp[127];
	int devh1 = devh/2;
	
	memset( tpt, 0, (sizeof( DV_TPT ) * 2) );

	/* Terminate GetDigits on Receiving MAXDTMF Digits */
	tpt[ 0 ].tp_type   = IO_CONT;
	tpt[ 0 ].tp_termno = DX_MAXDTMF;
	tpt[ 0 ].tp_length = 1;
	tpt[ 0 ].tp_flags  = TF_MAXDTMF;

	/* Terminate GetDigits after 8 Seconds */
	tpt[ 1 ].tp_type   = IO_EOT;
	tpt[ 1 ].tp_termno = DX_MAXTIME;
	tpt[ 1 ].tp_length = 20 * g_ivrperdig;
	tpt[ 1 ].tp_flags  = TF_MAXTIME;

	if( dx_getdig( oPort[devh1].voiceh, tpt, &(oPort[devh1].digbuf), EV_ASYNC ) == -1 ){
		sprintf( tmp, "dx_getdig( oPort[%d].voiceh, tpt, &(oPort[%d].digbuf), EV_ASYNC ) return -1", devh1, devh1 );
		sys_update_log( devh1, LOG_ERR, tmp );
	}
	
}


void chkservice(int devh)
{
	
	int devh1 = devh/2;
	char digits[50], tmp[127];
	int nRetval;
	char cStatement[500];
	char cName[67];
	SQLINTEGER cb_cName = SQL_NTS;
	SQLHSTMT hstmt;
	SQLINTEGER nMobid = 0,cb_nMobid = 0;	
	SQLCHAR cNRIC[21];
	SQLINTEGER cb_cNRIC = SQL_NTS;
	
	strcpy( digits, oPort[devh1].digbuf.dg_value );
	sprintf( tmp, "Get service completed %s", digits );
	sys_update_log( devh1, LOG_EVT, tmp );

	if( hdbc == NULL ){
		oPort[devh1].state = ST_NOTMOB;
		(*playlong)( devh );
		return;
	}

	switch( digits[0] ){
	case '2':
		oPort[devh1].nCallresult = 10; // Call Enter '2'
		if( oPort[devh1].cContactno[0] == ' ' || oPort[devh1].cContactno[0] == '\0' ){
			oPort[devh1].cExcuseflg[0] = 'Y';
			oPort[devh1].cCalltype = '2';
			oPort[devh1].state = ST_ENDCALL;
			(*playlong)( devh );
			break;
		}

		nRetval = SQLAllocHandle( SQL_HANDLE_STMT, hdbc,&hstmt );
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return;
		}
		strcpy( oPort[devh1].cName, "" );
		
		sprintf(cStatement,"select mob_id,name,nric from mobnominalroll (nolock) where cn1st = '%s' or cn2nd = '%s' or cn3rd = '%s' or cn4th = '%s' or cn5th = '%s' or cn6th = '%s' order by mob_id",
			oPort[devh1].cContactno, oPort[devh1].cContactno, oPort[devh1].cContactno, 
			oPort[devh1].cContactno, oPort[devh1].cContactno, oPort[devh1].cContactno );
		if( oPort[devh1].nWhichcontactno == 1 ){
			sprintf(cStatement,"select mob_id,name,nric from mobnominalroll (nolock) where cn1st = '%s'",
				oPort[devh1].cContactno );
		}
		if( oPort[devh1].nWhichcontactno == 2 ){
			sprintf(cStatement,"select mob_id,name,nric from mobnominalroll (nolock) where cn2nd = '%s'",
				oPort[devh1].cContactno );
		}
		if( oPort[devh1].nWhichcontactno == 3 ){
			sprintf(cStatement,"select mob_id,name,nric from mobnominalroll (nolock) where cn3rd = '%s'",
				oPort[devh1].cContactno );
		}
		if( oPort[devh1].nWhichcontactno == 4 ){
			sprintf(cStatement,"select mob_id,name,nric from mobnominalroll (nolock) where cn4th = '%s'",
				oPort[devh1].cContactno );
		}
		if( oPort[devh1].nWhichcontactno == 5 ){
			sprintf(cStatement,"select mob_id,name,nric from mobnominalroll (nolock) where cn5th = '%s'",
				oPort[devh1].cContactno );
		}
		if( oPort[devh1].nWhichcontactno == 6 ){
			sprintf(cStatement,"select mob_id,name,nric from mobnominalroll (nolock) where cn6th = '%s'",
				oPort[devh1].cContactno );
		}
		sys_update_log( 0, LOG_SQL, cStatement );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)&cStatement,SQL_NTS);
		nRetval = SQLBindCol(hstmt,1,SQL_C_SLONG,&nMobid,0,&cb_nMobid);
		nRetval = SQLBindCol(hstmt,2,SQL_C_CHAR,&cName,67,&cb_cName);
		nRetval = SQLBindCol(hstmt,3,SQL_C_CHAR,&cNRIC,21,&cb_cNRIC);
		
		strcpy( cName, "" ); strcpy( cNRIC, "" );
		nRetval = SQLFetch( hstmt );
		SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		cName[66] = '\0';
		cNRIC[20] = '\0';
		strcpy( oPort[devh1].cName, cName );
		

		if( oPort[devh1].nWhichcontactno < 1 || oPort[devh1].nWhichcontactno > 6 ){
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}
			if( oPort[devh1].cContactno[0] == '8' || oPort[devh1].cContactno[0] == '9' ){
				sprintf( cStatement, "update mobnominalroll set cn1st_mode = '2', sms_dttm = getdate(), e2_dttm = getdate() where cn1st = '%s'",
					oPort[devh1].cContactno );
			}
			else{
				sprintf( cStatement, "update mobnominalroll set cn1st_mode = '2', sms_dttm = getdate() where cn1st = '%s'",
					oPort[devh1].cContactno );
			}
			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}
			if( oPort[devh1].cContactno[0] == '8' || oPort[devh1].cContactno[0] == '9' ){
				sprintf( cStatement, "update mobnominalroll set cn2nd_mode = '2', sms_dttm = getdate(), e2_dttm = getdate() where cn2nd = '%s'",
					oPort[devh1].cContactno );
			}
			else{
				sprintf( cStatement, "update mobnominalroll set cn2nd_mode = '2', sms_dttm = getdate() where cn2nd = '%s'",
					oPort[devh1].cContactno );
			}
			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}
			if( oPort[devh1].cContactno[0] == '8' || oPort[devh1].cContactno[0] == '9' ){
				sprintf( cStatement, "update mobnominalroll set cn3rd_mode = '2', sms_dttm = getdate(), e2_dttm = getdate() where cn3rd = '%s'",
					oPort[devh1].cContactno );
			}
			else{
				sprintf( cStatement, "update mobnominalroll set cn3rd_mode = '2', sms_dttm = getdate() where cn3rd = '%s'",
					oPort[devh1].cContactno );
			}
			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}
			if( oPort[devh1].cContactno[0] == '8' || oPort[devh1].cContactno[0] == '9' ){
				sprintf( cStatement, "update mobnominalroll set cn4th_mode = '2', sms_dttm = getdate(), e2_dttm = getdate() where cn4th = '%s'",
					oPort[devh1].cContactno );
			}
			else{
				sprintf( cStatement, "update mobnominalroll set cn4th_mode = '2', sms_dttm = getdate() where cn4th = '%s'",
					oPort[devh1].cContactno );
			}
			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}
			if( oPort[devh1].cContactno[0] == '8' || oPort[devh1].cContactno[0] == '9' ){
				sprintf( cStatement, "update mobnominalroll set cn5th_mode = '2', sms_dttm = getdate(), e2_dttm = getdate() where cn5th = '%s'",
					oPort[devh1].cContactno );
			}
			else{
				sprintf( cStatement, "update mobnominalroll set cn5th_mode = '2', sms_dttm = getdate() where cn5th = '%s'",
					oPort[devh1].cContactno );
			}
			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}
			if( oPort[devh1].cContactno[0] == '8' || oPort[devh1].cContactno[0] == '9' ){
				sprintf( cStatement, "update mobnominalroll set cn6th_mode = '2', sms_dttm = getdate(), e2_dttm = getdate() where cn6th = '%s'",
					oPort[devh1].cContactno );
			}
			else{
				sprintf( cStatement, "update mobnominalroll set cn6th_mode = '2', sms_dttm = getdate() where cn6th = '%s'",
					oPort[devh1].cContactno );
			}
			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		else{
			nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
			if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
				hdbc = NULL;
				return;
			}
			if( oPort[devh1].nWhichcontactno == 1 ){
				if( oPort[devh1].cContactno[0] == '8' || oPort[devh1].cContactno[0] == '9' ){
					sprintf( cStatement, "update mobnominalroll set cn1st_mode = '2', sms_dttm = getdate(), e2_dttm = getdate() where cn1st = '%s'",
						oPort[devh1].cContactno );
				}
				else{
					sprintf( cStatement, "update mobnominalroll set cn1st_mode = '2', sms_dttm = getdate() where cn1st = '%s'",
						oPort[devh1].cContactno );
				}
			}
			if( oPort[devh1].nWhichcontactno == 2 ){
				if( oPort[devh1].cContactno[0] == '8' || oPort[devh1].cContactno[0] == '9' ){
					sprintf( cStatement, "update mobnominalroll set cn2nd_mode = '2', sms_dttm = getdate(), e2_dttm = getdate() where cn2nd = '%s'",
						oPort[devh1].cContactno );
				}
				else{
					sprintf( cStatement, "update mobnominalroll set cn2nd_mode = '2', sms_dttm = getdate() where cn2nd = '%s'",
						oPort[devh1].cContactno );
				}
			}
			if( oPort[devh1].nWhichcontactno == 3 ){
				if( oPort[devh1].cContactno[0] == '8' || oPort[devh1].cContactno[0] == '9' ){
					sprintf( cStatement, "update mobnominalroll set cn3rd_mode = '2', sms_dttm = getdate(), e2_dttm = getdate() where cn3rd = '%s'",
						oPort[devh1].cContactno );
				}
				else{
					sprintf( cStatement, "update mobnominalroll set cn3rd_mode = '2', sms_dttm = getdate() where cn3rd = '%s'",
						oPort[devh1].cContactno );
				}
			}
			if( oPort[devh1].nWhichcontactno == 4 ){
				if( oPort[devh1].cContactno[0] == '8' || oPort[devh1].cContactno[0] == '9' ){
					sprintf( cStatement, "update mobnominalroll set cn4th_mode = '2', sms_dttm = getdate(), e2_dttm = getdate() where cn4th = '%s'",
						oPort[devh1].cContactno );
				}
				else{
					sprintf( cStatement, "update mobnominalroll set cn4th_mode = '2', sms_dttm = getdate() where cn4th = '%s'",
						oPort[devh1].cContactno );
				}
			}
			if( oPort[devh1].nWhichcontactno == 5 ){
				if( oPort[devh1].cContactno[0] == '8' || oPort[devh1].cContactno[0] == '9' ){
					sprintf( cStatement, "update mobnominalroll set cn5th_mode = '2', sms_dttm = getdate(), e2_dttm = getdate() where cn5th = '%s'",
						oPort[devh1].cContactno );
				}
				else{
					sprintf( cStatement, "update mobnominalroll set cn5th_mode = '2', sms_dttm = getdate() where cn5th = '%s'",
						oPort[devh1].cContactno );
				}
			}
			if( oPort[devh1].nWhichcontactno == 6 ){
				if( oPort[devh1].cContactno[0] == '8' || oPort[devh1].cContactno[0] == '9' ){
					sprintf( cStatement, "update mobnominalroll set cn6th_mode = '2', sms_dttm = getdate(), e2_dttm = getdate() where cn6th = '%s'",
						oPort[devh1].cContactno );
				}
				else{
					sprintf( cStatement, "update mobnominalroll set cn6th_mode = '2', sms_dttm = getdate() where cn6th = '%s'",
						oPort[devh1].cContactno );
				}
			}

			sys_update_log( 0, LOG_SQL, cStatement );
			nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
			nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
			nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		}
		
		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return;
		}
		if( oPort[devh1].switched == INCOMING ){
			sprintf( cStatement, "insert into unsubscribe (nric,name,contact,mob_id,call_type,rec_dttm) values ('%s','%s','%s',%d,'I',getdate())",
				cNRIC, cName, oPort[devh1].cContactno, nMobid );
		}
		else{
			sprintf( cStatement, "insert into unsubscribe (nric,name,contact,mob_id,call_type,rec_dttm) values ('%s','%s','%s',%d,'O',getdate())",
				cNRIC, cName, oPort[devh1].cContactno, nMobid );
		}
		sys_update_log( 0, LOG_SQL, cStatement );
		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
		nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

		oPort[devh1].cExcuseflg[0] = 'Y';
		oPort[devh1].cCalltype = '2';
		oPort[devh1].state = ST_ENDCALL;
		(*playlong)( devh );
		break;

	case '1':
		oPort[devh1].cExcuseflg[0] = 'N';
		oPort[devh1].noofretry = 0;
		oPort[devh1].state = ST_NRIC;
		(*play)( devh );
		break;
	
	default:
		if( strlen( oPort[devh1].digbuf.dg_value ) > 0 ){
			if( oPort[devh1].noofretry > g_ivrnoretry - 1 ){
				oPort[devh1].state = ST_MAXRETRY;
				(*playlong)( devh );
			}
			else{
				oPort[devh1].noofretry = oPort[devh1].noofretry + 1;
				oPort[devh1].state = ST_WRONGSERVICE;
				(*playlong)( devh );
				
			}
		}
		else{
			if( oPort[devh1].noofretry > g_ivrnoretry - 1 ){
				oPort[devh1].state = ST_MAXRETRY;
				(*playlong)(devh);
			}
			else{
				oPort[devh1].noofretry = oPort[devh1].noofretry + 1;
				oPort[devh1].state = ST_NOSERVICE;
				(*playlong)( devh );
			}
		}
		break;
	}
}


void getlanguage(int devh)
{
	char tmp[127];
	int devh1 = devh/2;
	
	memset( tpt, 0, (sizeof( DV_TPT ) * 2) );

	/* Terminate GetDigits on Receiving MAXDTMF Digits */
	tpt[ 0 ].tp_type   = IO_CONT;
	tpt[ 0 ].tp_termno = DX_MAXDTMF;
	tpt[ 0 ].tp_length = 1;
	tpt[ 0 ].tp_flags  = TF_MAXDTMF;

	/* Terminate GetDigits after 8 Seconds */
	tpt[ 1 ].tp_type   = IO_EOT;
	tpt[ 1 ].tp_termno = DX_MAXTIME;
	tpt[ 1 ].tp_length = 20 * g_ivrperdig;
	tpt[ 1 ].tp_flags  = TF_MAXTIME;

	if( dx_getdig( oPort[devh1].voiceh, tpt, &(oPort[devh1].digbuf), EV_ASYNC ) == -1 ){
		sprintf( tmp, "dx_getdig( oPort[%d].voiceh, tpt, &(oPort[%d].digbuf), EV_ASYNC ) return -1", devh1, devh1 );
		sys_update_log( devh1, LOG_ERR, tmp );
	}
	
}


void chklanguage(int devh)
{
	int devh1 = devh/2;
	char digits[50];
	char tmp[127];

	strcpy( digits, oPort[devh1].digbuf.dg_value );
	sprintf( tmp, "Get language completed %s", digits );
	sys_update_log( devh1, LOG_EVT, tmp );

	switch( digits[0] ){
	case '1':
		oPort[devh1].noofretry = 0;
		oPort[devh1].nLanguage = 1;
		oPort[devh1].nAcksms = 0;
		oPort[devh1].state = ST_SERVICE;
		(*play)( devh );
		break;

	case '2':
		oPort[devh1].noofretry = 0;
		oPort[devh1].nLanguage = 2;
		oPort[devh1].nAcksms = 0;
		oPort[devh1].state = ST_SERVICE;
		(*play)( devh );
		break;
	
	default:
		if( strlen( oPort[devh1].digbuf.dg_value ) > 0 ){
			if( oPort[devh1].noofretry > g_ivrnoretry - 1 ){
				oPort[devh1].state = ST_MAXRETRY;
				(*playlong)(devh);
			}
			else{
				oPort[devh1].noofretry = oPort[devh1].noofretry + 1;
				oPort[devh1].state = ST_WRONGLANGUAGE;
				(*playlong)( devh );
				
			}
		}
		else{
			if( oPort[devh1].noofretry > g_ivrnoretry - 1 ){
				oPort[devh1].state = ST_MAXRETRY;
				(*playlong)(devh);
			}
			else{
				oPort[devh1].noofretry = oPort[devh1].noofretry + 1;
				oPort[devh1].state = ST_NOLANGUAGE;
				(*playlong)( devh );
			}
		}
		break;
	}
}

void getacksms(int devh)
{
	char tmp[127];
	int devh1 = devh/2;
	
	memset( tpt, 0, (sizeof( DV_TPT ) * 2) );

	/* Terminate GetDigits on Receiving MAXDTMF Digits */
	tpt[ 0 ].tp_type   = IO_CONT;
	tpt[ 0 ].tp_termno = DX_MAXDTMF;
	tpt[ 0 ].tp_length = 1;
	tpt[ 0 ].tp_flags  = TF_MAXDTMF;

	/* Terminate GetDigits after 8 Seconds */
	tpt[ 1 ].tp_type   = IO_EOT;
	tpt[ 1 ].tp_termno = DX_MAXTIME;
	tpt[ 1 ].tp_length = 20 * g_ivrperdig;
	tpt[ 1 ].tp_flags  = TF_MAXTIME;

	if( dx_getdig( oPort[devh1].voiceh, tpt, &(oPort[devh1].digbuf), EV_ASYNC ) == -1 ){
		sprintf( tmp, "dx_getdig( oPort[%d].voiceh, tpt, &(oPort[%d].digbuf), EV_ASYNC ) return -1", devh1, devh1 );
		sys_update_log( devh1, LOG_ERR, tmp );
	}
	
}


void chkacksms(int devh)
{
	
	int devh1 = devh/2;
	char digits[50], tmp[127];
	
	strcpy( digits, oPort[devh1].digbuf.dg_value );
	sprintf( tmp, "Get acksms completed %s", digits );
	sys_update_log( devh1, LOG_EVT, tmp );

	
	switch( digits[0] ){
	case '1':
		oPort[devh1].state = ST_MOBILENO;
		(*play)( devh );
		break;
		
	default:
		if( strlen( oPort[devh1].digbuf.dg_value ) > 0 ){
			oPort[devh1].state = ST_ACK1;
			strcpy( oPort[devh1].playnumber, oPort[devh1].cOutstanding );
			oPort[devh1].playno = 0;
			(*playlong)( devh );
		}
		else{
			if( oPort[devh1].noofretry > g_ivrnoretry - 1 ){
				oPort[devh1].state = ST_MAXRETRY;
				(*playlong)(devh);
			}
			else{
				oPort[devh1].noofretry = oPort[devh1].noofretry + 1;
				oPort[devh1].state = ST_NOACKSMS;
				(*playlong)( devh );
			}
		}
		break;
	}
}

void getmobileno(int devh)
{
	char tmp[127];
	int devh1 = devh/2;
	/*
	 * Clear and then Set the DV_TPT structures
	 */
	memset( tpt, 0, (sizeof( DV_TPT ) * 3) );

	/* Terminate GetDigits on Receiving MAXDTMF Digits */
	tpt[ 0 ].tp_type   = IO_CONT;
	tpt[ 0 ].tp_termno = DX_DIGMASK;
	tpt[ 0 ].tp_length = DM_P;
	tpt[ 0 ].tp_flags  = TF_LEVEL;

	/* Terminate GetDigits on Receiving MAXDTMF Digits */
	tpt[ 1 ].tp_type   = IO_CONT;
	tpt[ 1 ].tp_termno = DX_MAXDTMF;
	tpt[ 1 ].tp_length = 25;
	tpt[ 1 ].tp_flags  = TF_MAXDTMF;

	/* Terminate GetDigits after 8 Seconds */
	tpt[ 2 ].tp_type   = IO_EOT;
	tpt[ 2 ].tp_termno = DX_MAXTIME;
	tpt[ 2 ].tp_length = 100 * g_ivrperdig;
	tpt[ 2 ].tp_flags  = TF_MAXTIME;

	if( dx_getdig( oPort[devh1].voiceh, tpt, &(oPort[devh1].digbuf), EV_ASYNC ) == -1 ){
		sprintf( tmp, "dx_getdig( oPort[%d].voiceh, tpt, &(oPort[%d].digbuf), EV_ASYNC ) return -1", devh1, devh1 );
		sys_update_log( devh1, LOG_ERR, tmp );
	}
}

void chkmobileno(int devh)
{
	int devh1, digit_length = 0;
	char digits[50], tmp[127];
	int i;

	devh1 = devh/2;
	strcpy( digits, oPort[devh1].digbuf.dg_value );
	sprintf( tmp, "Get mobileno completed %s", digits );
	sys_update_log( devh1, LOG_EVT, tmp );

	digit_length = strcspn( digits, "#" );
	//strcpy( oPort[devh1].cCalldigits, digits );

	for( i=0; i<digit_length; i++ ){
		if( digits[i] == '*' ){
			digit_length = 0;
			break;
		}
	}

	if( digit_length > 8 || digit_length < 8 ){
		
		if( oPort[devh1].noofmobileno >= g_ivrnoretry - 1 ){
			oPort[devh1].state = ST_MAXRETRY;
			(*playlong)( devh );
			return;
		}
		else{
			oPort[devh1].noofmobileno = oPort[devh1].noofmobileno + 1 ;
			if( strlen( digits ) == 0 ){
				oPort[devh1].state = ST_NOMOBILENO;
				(*playlong)( devh );
			}
			else{
				oPort[devh1].state = ST_WRONGMOBILENO;
				(*playlong)( devh );
			}
		}
	}
	else{
		if( digits[0] == '8' || digits[0] == '9' ){
			digits[digit_length] ='\0';
			strcpy( oPort[devh1].playnumber, digits );
			strcpy( oPort[devh1].cAckMobileno, digits );
			oPort[devh1].playno = 0;

			oPort[devh1].state = ST_REPEATMOBILE;
			(*playlong)( devh );
		}
		else{
			if( oPort[devh1].noofmobileno >= g_ivrnoretry - 1 ){
				oPort[devh1].state = ST_MAXRETRY;
				(*playlong)( devh );
				return;
			}
			else{
				oPort[devh1].noofmobileno = oPort[devh1].noofmobileno + 1 ;
				if( strlen( digits ) == 0 ){
					oPort[devh1].state = ST_NOMOBILENO;
					(*playlong)( devh );
				}
				else{
					oPort[devh1].state = ST_WRONGMOBILENO;
					(*playlong)( devh );
				}
			}
		}
	}
}

void chkconfmno(int devh)
{
	int devh1 = devh/2;
	char digits[50], tmp[127];
	int nRetval;
	char cStatement[270];
	SQLHSTMT hstmt;

	strcpy( digits, oPort[devh1].digbuf.dg_value );
	sprintf( tmp, "Get digits completed %s", digits );
	sys_update_log( devh1, LOG_EVT, tmp );
	
	if( hdbc == NULL ){
		oPort[devh1].state = ST_NOTMOB;
		(*playlong)( devh );
		return;
	}

	switch( digits[0] ){
	case '1':
		nRetval = SQLAllocHandle(SQL_HANDLE_STMT,hdbc,&hstmt);
		if( nRetval == SQL_INVALID_HANDLE || nRetval == SQL_ERROR ){
			hdbc = NULL;
			return;
		}
		
		sprintf( cStatement, "update mobnominalroll set register_status = 'P', register_no = '%s', appointment = '1', noofretry = 0 where mob_id = %d and pin = '%s'",
			oPort[devh1].cAckMobileno, oPort[devh1].nMobid, oPort[devh1].cPin );

		nRetval = SQLExecDirect(hstmt,(unsigned char *)cStatement,SQL_NTS);
		nRetval = SQLEndTran(SQL_HANDLE_DBC,hdbc,SQL_COMMIT);
		nRetval = SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		oPort[devh1].nAdhocid = 1;
		oPort[devh1].state = ST_RECVSMS;
		(*playlong)( devh );
		break;

	case '0':
		if( oPort[devh1].noofretry > g_ivrnoretry - 1 ){
			oPort[devh1].state = ST_MAXRETRY;
			(*playlong)( devh );
		}
		else{
			oPort[devh1].noofretry = oPort[devh1].noofretry + 1;
			oPort[devh1].state = ST_MOBILENO;
			(*play)( devh );
		}
		break;

	case '3':
		if( oPort[devh1].noofretry > g_ivrnoretry - 1 ){
			oPort[devh1].state = ST_MAXRETRY;
			(*playlong)( devh );
		}
		else{
			oPort[devh1].noofretry = oPort[devh1].noofretry + 1;
			oPort[devh1].playno = 0;
			oPort[devh1].state = ST_REPEATMOBILE;
			(*playlong)( devh );
		}
		break;

	default:
		if( strlen( oPort[devh1].digbuf.dg_value ) > 0 ){
			if( oPort[devh1].noofretry > g_ivrnoretry - 1 ){
				oPort[devh1].state = ST_MAXRETRY;
				(*playlong)( devh );
			}
			else{
				oPort[devh1].noofretry = oPort[devh1].noofretry + 1;
				oPort[devh1].state = ST_WRONGCONFMNO;
				(*playlong)( devh );
				
			}
		}
		else{
			if( oPort[devh1].noofretry > g_ivrnoretry - 1 ){
				oPort[devh1].state = ST_MAXRETRY;
				(*playlong)(devh);
			}
			else{
				oPort[devh1].noofretry = oPort[devh1].noofretry + 1;
				oPort[devh1].state = ST_NOCONFMNO;
				(*playlong)( devh );
			}
		}
		break;
	}
}
