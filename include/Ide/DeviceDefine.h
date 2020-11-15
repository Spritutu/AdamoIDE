#if !defined(DEVICEDEFINE)
#define DEVICEDEFINE

#include <float.h>

#define ID_NAME                 0
#define ID_DESCRIPTION          1
#define ID_ID                   2
//Axis
#define ID_RESOLUTION           3
#define ID_AXISTYPE             4
#define ID_AXESCATEGORY         5
#define ID_UNIT                 6
#define ID_INVPHASE             7
#define ID_INVRIF               8
#define ID_ENABLENICK           9   
#define ID_ENABLEJERK           10  
#define ID_SPEED                11
#define ID_SPEEDMAN             12
#define ID_ACC                  13
#define ID_DECC                 14
#define ID_PROP                 15
#define ID_INT                  16
#define ID_DER                  17
#define ID_FEEDFORWARD          18
#define ID_FEEDFORWARDACC       19
#define ID_RIF                  20
#define ID_RAMPTYPE             21

#define ID_INTERSPEED           22
#define ID_INTERACC             23
#define ID_INTERDEC             24
#define ID_INTERPROP            25
#define ID_INTERINT             26
#define ID_INTERDER             27
#define ID_INTERFEEDFORWARD     28
#define ID_INTERFEEDFORWARDACC  29
#define ID_INTERRAMPTYPE        30

#define ID_WAITSTOPAXIS         31
#define ID_LIMEACONC            32
#define ID_ENABLELIMAXESPOS     33
#define ID_LIMEADISC            34
#define ID_ENABLELIMAXESNEG     35
#define ID_LIMAXISPOS			36
#define ID_LIMAXISNEG			37
#define ID_WINPOS				38
#define ID_WINNEG				39
#define ID_ALIAS                40
#define ID_SERVOLIMITTYPE       41
#define ID_CC                   42
#define ID_CP                   43
#define ID_AXISVIRTUAL          44
#define ID_GNAME                45
#define ID_GSLOT                46
#define ID_GANTRYMASTER         47
#define ID_GANTRYPROP           48
#define ID_GANTRYDER            49
#define ID_GANTRYINT            50

//AnalogIn
#define ID_LOGICRANGEMIN        51
#define ID_LOGICRANGEMAX        52
//DgtOut
#define ID_ISMONOSTABLE         53
#define ID_OUTNUMBER            54
#define ID_OUTTIMEOUT           55
//Counter
#define ID_COUNTERTYPE          56
//Timer
#define ID_PERIOD               57
#define ID_UPDOWN               58
#define ID_ENABLED              59
#define ID_STORED               60
// moduli
#define ID_MACHINE_TYPE         61  
#define ID_MACHINE_NUMBER       62
#define ID_MACHINE_FREQUENCY    63

#define ID_PERCRT				64
#define ID_INTERPERCRT			65
#define ID_DIMBUGINTEG			66

#define ID_RATE                 67

#define ID_DISPLAY              68

#define ID_TICVELOCITA          69
#define ID_AXISTIC              70
#define ID_AXISMOVETYPE         71
#define ID_PSWLEVELR            72
#define ID_PSWLEVELW            73

#define ID_PRECISION            74
#define ID_RADIUSPRECISION      75

#define ID_WAITONACCCHANGE      76
#define ID_MESSAGEFILTERINGONSTART 77
#define ID_STOPAXISONERROR      78

#define ID_ENABLE_COMPENSAZIONE_GIOCO     79
#define ID_COMPENSAZIONE_GIOCO  80
#define ID_TEMPO_COMPENSAZIONE  81

#define ID_ENABLE_COMPENSAZIONE_EA     82
#define ID_COMPENSAZIONE_EA  83

#define ID_RANGEQUOTAPOS  84
#define ID_RANGEQUOTANEG  85

#define ID_MAXPERCTORQUE  86

#define ID_ROTATIONFILTER  87

#define ID_5ASSI_SMOOTHING  88

#define ID_TEMPOARRIVOINQUOTA   89
#define ID_TEMPOASSEFERMO       90

#define ID_INVERTIG2G3		91
#define ID_KAC				92

#define ID_VELOCITADICORREZIONE		93
#define ID_CORREZIONEDINAMICA		94
#define ID_KLC				95

#define ID_LIMEAMIN            96
#define ID_5ASSI_USE_AS_DEFAULT 97
#define ID_5ASSI_SPEED 98
#define ID_5ASSI_ACC 99
#define ID_5ASSI_DEC 100
#define ID_5ASSI_BOW_THICKENING 101
#define ID_5ASSI_LINEAR_THICKENING 102
#define ID_5ASSI_SINGLESPIKEFACTOR 103
#define ID_5ASSI_GEOMETRYSPIKEFACTOR 104
#define ID_5ASSI_SPIKEDETECTOR 105
#define ID_5ASSI_MAXUTURNPRECISION 106
#define ID_5ASSI_SMOOTHERTYPE 107
#define ID_5ASSI_SINGLESPIKESMOOTHERWEIGHT 108
#define ID_5ASSI_DETECTJOIUNTUTURN 109
#define ID_5ASSI_STOPONANGLE 110
#define ID_5ASSI_MINSTOPANGLE 111

#define ID_RAM_DISK	112
#define ID_RAM_DISK_BUFFER_SIZE	113

#define ID_G0RAGGIATA_ENABLE		114
#define ID_G0RAGGIATA_INFITTIMENTO	115
#define ID_G0RAGGIATA_RAGGIO		116
#define ID_SIMULATIONMODE			117
#define ID_MAXJITTER				118
#define ID_CONTOURANGLE				119
#define ID_CENTROINCREMENTALE		120
	
#define ID_INTKT					121
#define ID_ENABLESWITCHEDINT		122
#define ID_SWITCHEDINTSPEED			123
#define ID_DERN						124
#define ID_INTERPINTKT				125
#define ID_INTERENABLESWITCHEDINT	126
#define ID_INTERSWITCHEDINTSPEED	127
#define ID_INTERDERN				128
#define ID_FILTROCOLLENCODER		129
#define ID_FILTROSERVOERROR			130
#define ID_BOOSTPLC					131
#define ID_5ASSI_SIP				132
#define ID_KYNEMATIC_COMPENSATIONS	133
#define ID_STRICTABSPOSCONTROL		134
#define ID_STRICTABSPOSENCSTEPS		135
#define ID_COMPENSAZIONE_EAA		136


//////////////////////////////////////////
//          RANGE PROPERTY              //
//////////////////////////////////////////

#define MAXVALUERESOLUTION      DBL_MAX
#define MAXVALUE                1.0E6
#define MINVALUE                -1.0E6
#define ZERO                    0.0
#define RIFVOLTAGE_NEG          -10.0
#define RIFVOLTAGE_POS          10.0
#define MAXADDRESS              0xFFFF
#define MAXINOUT                64
#endif // !defined(DEVICEDEFINE)
