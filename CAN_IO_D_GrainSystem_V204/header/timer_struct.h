//Mappa clock
typedef struct 
{
	uint8_t t_Clock100ms;
	uint8_t t_Clock250ms;
	uint8_t t_Clock500ms;
	uint8_t t_Clock1000ms;

	unsigned s_Clock100ms: 1;
	unsigned s_Clock250ms: 1;
	unsigned s_Clock500ms: 1;
	unsigned s_Clock1000ms: 1;
	unsigned s_Clock100msPrec: 1;
	unsigned s_Clock250msPrec: 1;
	unsigned s_Clock500msPrec: 1;
	unsigned s_Clock1000msPrec: 1;

	unsigned imp_Clock100ms: 1;
	unsigned imp_Clock250ms: 1;
	unsigned imp_Clock500ms: 1;
	unsigned imp_Clock1000ms: 1;
	unsigned imp_b04: 1;
	unsigned imp_b05: 1;
	unsigned imp_b06: 1;
	unsigned imp_b07: 1;
} myTimer;
