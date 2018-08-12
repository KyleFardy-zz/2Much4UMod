//Menu base created by 2much4u modified by PhucedMODZ

#include <cellstatus.h>
#include <sys/prx.h>
#include <sys/ppu_thread.h>
#include <string.h>
#include <sys/memory.h>
#include <sys/timer.h>
#include <sys/process.h>
#include <ppu_intrinsics.h>
#include <sys/return_code.h>
#include <sysutil/sysutil_oskdialog.h>
#include <sysutil/sysutil_msgdialog.h>
#include <stdarg.h>
#include <stdio.h>
#include <cstdlib>

#include <sys/sys_time.h>
#include <sys/time_util.h>
#include <assert.h>
#include <stddef.h>
#include <math.h>
#include <typeinfo>
#include <vector>
#include <pthread.h>
#include <locale.h>
#include <cell/error.h>
#include <sys/paths.h>
#include <time.h>
#include <cell/cell_fs.h>
#include <cell/sysmodule.h>
#include <cell/fs/cell_fs_errno.h>
#include <cell/fs/cell_fs_file_api.h>
#include <vector>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <net\if_dl.h>
#include <arpa/inet.h>
#include <np.h>

#include "math.h"
#include "Enums.h"
#include "Natives.h"
#include "Dialog.h"

SYS_MODULE_INFO("GTAV_Native_Invoker", 0, 1, 1);
SYS_MODULE_START(PRX_ENTRY);

#pragma region Variables
Native_s** g_Natives;
int submenu = 0;
int submenuLevel;
int lastSubmenu[20];
int lastOption[20];
int currentOption;
int optionCount;
bool optionPress = false;
bool ShowTalkingPlayers = false;
char* talkinglayer;
bool rightPress = false;
bool leftPress = false;
bool fastRightPress = false;
bool fastLeftPress = false;
bool squarePress = false;
int selectedPlayer;
bool menuSounds = true;
bool keyboardActive = false;
int keyboardAction;
int *keyboardVar = 0;
char *infoText;
bool Movement, decrease, increase, movingTitle = true;
float index;
int timerTitleDownUp = 0, timerDecrease = 0;
int menuDownUp = 0, menuDecrease = 0;
float titleScale = 0.75f;
bool godmode, invisibility;
bool newTimerTick = true;
int maxTimerCount;
bool newTimerTick2 = true;
int maxTimerCount2;
int gGlareHandle;
float gGlareDir;
float menuXCoord = 0.845f;
float textXCoord = 0.76f;
bool flashingTitle = true;

int fademe = 0;
int fademe2 = 0;
float FadeInSpeed = 0.005;
int FadeInSpeedint = 60;
bool fadeinbool = false;
bool ScrollAnimationBool = false;

float checkboxshit1 = 0.920, UpDown = .1135f; // 0.870f + .070f
bool selfoption[50];
bool instructions = true;
bool showGlare = true;
bool showCredits = false;
int instructCount;
int mov;
float scrollerVar = 0.962;
bool instructionsSetupThisFrame;
bool xInstruction;
bool squareInstruction;
bool lrInstruction;
int bannerTextRed = 255;
int bannerTextGreen = 255;
int bannerTextBlue = 255;
int bannerTextOpacity = 255;
int bannerTextFont = 7;
int bannerRectRed = 7;
int bannerRectGreen = 52;
int bannerRectBlue = 110;
int bannerRectOpacity = 255;
int backgroundRed = 16;
int backgroundGreen = 16;
int backgroundBlue = 16;
int backgroundOpacity = 70;
int optionsRed = 255;
int optionsGreen = 255;
int optionsBlue = 255;
int optionsOpacity = 255;
int optionsFont = 0;
int scrollerRed = 46;
int scrollerGreen = 186;
int scrollerBlue = 255;
int scrollerOpacity = 100;
int indicatorRed = 255;
int indicatorGreen = 44;
int indicatorBlue = 55;
int indicatorOpacity = 255;
int maxOptions = 20;
char opt[100];
char* menuName = "Menu Base";
bool welcomeShown;
#pragma endregion

#pragma region Internals
void HookNative(int native, int dest) {
	int FuncBytes[1];
	FuncBytes[0] = *(int*)dest;
	memcpy((void*)native, FuncBytes, 4);
}
void *Memcpy(void *dst0, const void *src0, size_t len0)
{
	char *dst = (char *)dst0;
	char *src = (char *)src0;
	void *save = dst0;
	while (len0--)
		*dst++ = *src++;
	return save;
}
int write_process(void* destination, const void* source, size_t size)
{
	Memcpy(destination, source, size);
}
void PatchInJump(uint64_t UIess, int Destination, bool Linked)
{
	int FuncBytes[4];
	Destination = *(int*)Destination;
	FuncBytes[0] = 0x3D600000 + ((Destination >> 16) & 0xFFFF);
	if (Destination & 0x8000)
		FuncBytes[0] += 1;
	FuncBytes[1] = 0x396B0000 + (Destination & 0xFFFF);
	FuncBytes[2] = 0x7D6903A6;
	FuncBytes[3] = 0x4E800420;
	if (Linked)
		FuncBytes[3] += 1;
	write_process((void*)UIess, FuncBytes, 4 * 4);
}
unsigned int FindNativeTableUIess()
{
	unsigned int  l_uiNativeTableUIess = 0;
	unsigned int l_uiStartUIess = 0x390000;
	unsigned int l_uiUIess = 0;
	for (unsigned int i = 0; i < 0x10000; i++)
	{
		if (*(unsigned int*)(l_uiStartUIess + i) == 0x3C6072BD)
			if (*(unsigned int*)(l_uiStartUIess + i + 8) == 0x6063E002)
			{
				l_uiUIess = *(unsigned int*)(l_uiStartUIess + i + 0x10);
				l_uiUIess &= 0xFFFFFF;
				l_uiUIess += (l_uiStartUIess + i + 0x10) - 1;
				printf("UIess: 0x%08x\n", l_uiUIess);
				break;
			}
	}
	l_uiNativeTableUIess = (*(unsigned int*)(l_uiUIess + 0x24) << 16) + (*(unsigned int*)(l_uiUIess + 0x2C) & 0xFFFF);
	l_uiNativeTableUIess -= 0x10000;
	return l_uiNativeTableUIess;
}
int NativeUIess(int Native, bool PatchInJump = true)
{
	int Hash = Native & 0xFF;
	int Table = *(int*)(FindNativeTableUIess() + (Hash * 4));
	while (Table != 0)
	{
		int NativeCount = *(int*)(Table + 0x20);
		for (int i = 0; i < NativeCount; i++)
		{
			if (*(int*)((Table + 0x24) + (i * 4)) == Native)
			{
				if (PatchInJump)
				{
					int NativeLocation = *(int*)(*(int*)((Table + 4) + (i * 4)));
					for (int i = 0; i < 50; ++i)
					{
						short CurrentPlace = *(short*)(NativeLocation + (i * 4));
						if (CurrentPlace == 0x4AE6 || CurrentPlace == 0x4AA8 || CurrentPlace == 0x4AE4 || CurrentPlace == 0x4AE5)
						{
							return (((*(int*)(NativeLocation + (i * 4))) - 0x48000001) + (NativeLocation + (i * 4))) - 0x4000000;
						}
					}
				}
				else
				{
					return *(int*)((Table + 4) + (i * 4));
				}

			}
		}
		Table = *(int*)(Table);
	}
}
void sleep(usecond_t time)
{
	sys_timer_usleep(time * 1000);
}
void set_text_component(char *text)
{
	GRAPHICS::_0x3AC9CB55("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	GRAPHICS::_0x386CE0B8();
}
void instructionsSetup()
{
	mov = GRAPHICS::REQUEST_SCALEFORM_MOVIE("instructional_buttons");
	GRAPHICS::_0x7B48E696(mov, 255, 255, 255, 0);
	GRAPHICS::_0x215ABBE8(mov, "CLEAR_ALL");
	GRAPHICS::_0x02DBF2D7();
	GRAPHICS::_0x215ABBE8(mov, "SET_CLEAR_SPACE");
	GRAPHICS::_0x716777CB(200);
	GRAPHICS::_0x02DBF2D7();
	instructCount = 0;
}
void addInstruction(int button, char *text)
{
	if (!instructionsSetupThisFrame)
	{
		instructionsSetup();
		instructionsSetupThisFrame = true;
	}
	GRAPHICS::_0x215ABBE8(mov, "SET_DATA_SLOT");
	GRAPHICS::_0x716777CB(instructCount);
	GRAPHICS::_0x716777CB(button);
	set_text_component(text);
	GRAPHICS::_0x02DBF2D7();
	instructCount++;
}
void instructionsClose()
{
	GRAPHICS::_0x215ABBE8(mov, "DRAW_INSTRUCTIONAL_BUTTONS");
	GRAPHICS::_0x02DBF2D7();
	GRAPHICS::_0x215ABBE8(mov, "SET_BACKGROUND_COLOUR");
	GRAPHICS::_0x716777CB(0);
	GRAPHICS::_0x716777CB(0);
	GRAPHICS::_0x716777CB(0);
	GRAPHICS::_0x716777CB(80);
	GRAPHICS::_0x02DBF2D7();
}
void drawTitleText(char * text, int font, float x, float y, float scalex, float scaley, int r, int b, int g, int a, bool center)
{
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(scalex, scaley);
	UI::SET_TEXT_COLOUR(r, g, b, a);
	UI::SET_TEXT_WRAP(0.0f, 1.0f);
	UI::SET_TEXT_CENTRE(center);
	UI::SET_TEXT_DROPSHADOW(3, 255, 0, 0, 255);
	UI::SET_TEXT_EDGE(1, 255, 0, 0, 255);
	UI::SET_TEXT_OUTLINE();
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DRAW_TEXT(x, y);
}
void drawText(char * text, int font, float x, float y, float scalex, float scaley, int r, int b, int g, int a, bool center)
{
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(scalex, scaley);
	UI::SET_TEXT_COLOUR(r, g, b, a);
	UI::SET_TEXT_WRAP(0.0f, 1.0f);
	UI::SET_TEXT_CENTRE(center);
	UI::SET_TEXT_DROPSHADOW(2, 2, 0, 0, 0);
	UI::SET_TEXT_OUTLINE();
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DRAW_TEXT(x, y);
}
void drawNotification(char* msg, char* subject)
{
	UI::_0x574EE85C("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(msg);
	UI::_SET_NOTIFICATION_MESSAGE("CHAR_SOCIAL_CLUB", "CHAR_SOCIAL_CLUB", true, 1, menuName, subject);
	UI::_DRAW_NOTIFICATION(false, true);
}
char byteArray[100];
char* ReadBytes(int UIess, int length)
{
	for (int i = 0; i < length; i++)
	{
		byteArray[i] = *(char*)(UIess + (i));
	}
	return byteArray;
}
void WriteBytes(int UIess, char* input, int length)
{
	for (int i = 0; i < length; i++)
	{
		*(char*)(UIess + (i)) = input[i];
	}
}
char returnRead[500];
char* ReadString(int UIess)
{
	memset(&returnRead[0], 0, sizeof(returnRead));
	int strlength = 100;
	char* StrBytes = ReadBytes(UIess, strlength);
	for (int i = 0; i < strlength; i++)
	{
		if (StrBytes[i] != 0x00)
			returnRead[i] = StrBytes[i];
		else
			break;
	}
	return returnRead;
}
void WriteString(int UIess, char* string)
{
	int FreeMem = 0x1D00000;
	int strlength = std::strlen(string);
	char* strpointer = *(char**)FreeMem = string;
	char* StrBytes = ReadBytes(*(int*)FreeMem, strlength);
	WriteBytes(UIess, StrBytes, strlength);
	*((char*)UIess + strlength) = '\0';
}
void playSound(char* sound)
{
	if (menuSounds)
		AUDIO::PLAY_SOUND_FRONTEND(-1, sound, "HUD_FRONTEND_DEFAULT_SOUNDSET");
}
bool isNumericChar(char x)
{
	return (x >= '0' && x <= '9') ? true : false;
}
int StoI(char *str)
{
	if (*str == NULL)
		return 0;

	int res = 0,
		sign = 1,
		i = 0;

	if (str[0] == '-')
	{
		sign = -1;
		i++;
	}
	for (; str[i] != '\0'; ++i)
	{
		if (isNumericChar(str[i]) == false)
			return 0;
		res = res * 10 + str[i] - '0';
	}
	return sign*res;
}
char *ItoS(int num)
{
	char buf[30];
	snprintf(buf, sizeof(buf), "%i", num);
	return buf;
}
char* FtoS(float input)
{
	char returnvalue[64];
	int wholenumber = (int)input;
	input -= wholenumber;
	input *= 100;
	sprintf(returnvalue, "%d.%d", wholenumber, (int)input);
	return returnvalue;
}
void startKeyboard(int action, char *defaultText, int maxLength)
{
	GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(0, "FMMC_KEY_TIP8", "", defaultText, "", "", "", maxLength);
	keyboardAction = action;
	keyboardActive = true;
}
void changeSubmenu(int newSubmenu)
{
	lastSubmenu[submenuLevel] = submenu;
	lastOption[submenuLevel] = currentOption;
	currentOption = 1;
	submenu = newSubmenu;
	submenuLevel++;
}
void SetupTimer(int tickTime)
{
	if (newTimerTick) {
		maxTimerCount = GAMEPLAY::GET_GAME_TIMER() + tickTime;
		newTimerTick = false;
	}
}
bool IsTimerReady()
{
	return GAMEPLAY::GET_GAME_TIMER() > maxTimerCount;
}
void ResetTimer()
{
	newTimerTick = true;
}
void SetupTimer2(int tickTime)
{
	if (newTimerTick2) {
		maxTimerCount2 = GAMEPLAY::GET_GAME_TIMER() + tickTime;
		newTimerTick2 = false;
	}
}
bool IsTimerReady2()
{
	return GAMEPLAY::GET_GAME_TIMER() > maxTimerCount2;
}

void ResetTimer2()
{
	newTimerTick2 = true;
}

void DRAW_TEXTURE(char *Streamedtexture, char *textureName, float x, float y, float width, float height, float rotation, int r, int g, int b, int a)
{
	if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED((char*)Streamedtexture))
	{
		GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT((char*)Streamedtexture, false);
	}
	else
	{
		GRAPHICS::DRAW_SPRITE((char*)Streamedtexture, (char*)textureName, x, y, width, height, rotation, r, g, b, a);
	}
}

bool x;
bool y;
bool width;
bool height;
bool rotation;
void DrawSprite(char  *Streamedtexture, char  *textureName, float x, float y, float width, float height, float rotation, int r, int g, int b, int a)
{
	if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(Streamedtexture))
		GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(Streamedtexture, false);
	else
		GRAPHICS::DRAW_SPRITE(Streamedtexture, textureName, x, y, width, height, rotation, r, g, b, a);
}
float conv360(float base, float min, float max) {

	float fVar0;
	if (min == max) return min;
	fVar0 = max - min;
	base -= round(base - min / fVar0) * fVar0;
	if (base < min) base += fVar0;
	return base;
}
int scaleform;
float GlareX = 1.1f;
float GlareY = 0.47;
float Glarewidth = 0.71f;
float Glareheight = 0.87f;
void drawGlare() {
	gGlareHandle = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MP_MENU_GLARE");
	GRAPHICS::DRAW_SCALEFORM_MOVIE(gGlareHandle, GlareX, GlareY, Glarewidth, Glareheight, 255, 255, 255, 255, 0);
}
void drawTalker(char* Text, int Index)
{
	UI::SET_TEXT_FONT(1);
	UI::SET_TEXT_SCALE(0.500, 0.500);
	UI::SET_TEXT_COLOUR(bannerTextRed, bannerTextGreen, bannerTextBlue, bannerTextOpacity);
	UI::SET_TEXT_OUTLINE();
	UI::SET_TEXT_WRAP(0, 1);
	UI::SET_TEXT_CENTRE(false);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(Text);
	UI::_DRAW_TEXT(0.070, 0.220 + (Index * 0.028));
}
void drawTextRightJustify(char * text, int font, float x, float y, float scalex, float scaley, int r, int g, int b, int a, int vari = 0)
{
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(scalex, scaley);
	UI::SET_TEXT_RIGHT_JUSTIFY(true);
	UI::SET_TEXT_WRAP(0.1f, .95f);
	UI::SET_TEXT_COLOUR(255, 255, 255, 255);
	if (currentOption == optionCount)
	{
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::_SET_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING(text);
		snprintf(opt, sizeof(opt), "%s", text);
	}
	else
	{
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::_SET_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING(text);;
	}
	UI::_DRAW_TEXT(x, y);
}
float menuXCoord3 = 0.915f;
void addTitle(char *title, char* subtitle)
{
	char titbuf[50];
	snprintf(titbuf, sizeof(titbuf), "~italic~%s", title);
	char* actitle = titbuf; 
	drawTitleText(actitle, bannerTextFont, menuXCoord, 0.090f, titleScale, titleScale, bannerTextRed, bannerTextGreen, bannerTextBlue, bannerTextOpacity, true);//was 0.090f
	GRAPHICS::DRAW_RECT(menuXCoord, 0.1175f, 0.19f, 0.083f, bannerRectRed, bannerRectGreen, bannerRectBlue, bannerRectOpacity);
	DRAW_TEXTURE("timerbars", "lhs_bg", menuXCoord, 0.1175f, 0.19f, 0.083f, 0, 46, 105, 186, bannerRectOpacity);
	drawText(subtitle, 4, menuXCoord, 0.125, 0.40, 0.40f, 255, 255, 255, 255, true);
	if (showGlare) {
		drawGlare();
	}
}
#pragma region Option Helpers
void DrawSpriteInMenuRed(char* p1, char* p2)
{
	if (currentOption <= maxOptions && optionCount <= maxOptions)
		DrawSprite(p1, p2, checkboxshit1, (optionCount * .035f + UpDown + .023200f), 0.035f, 0.05f, 0, 200, 0, 0, 255);
	else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
		DrawSprite(p1, p2, checkboxshit1, ((optionCount - (currentOption - maxOptions)) * .035f + UpDown + .023200f), 0.035f, 0.05f, 0, 200, 0, 0, 255);
}
void DrawSpriteInMenuGreen(char* p1, char* p2)
{
	if (currentOption <= maxOptions && optionCount <= maxOptions)
		DrawSprite(p1, p2, checkboxshit1, (optionCount * .035f + UpDown + .023200f), 0.035f, 0.05f, 0, 0, 179, 15, 255);
	else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
		DrawSprite(p1, p2, checkboxshit1, ((optionCount - (currentOption - maxOptions)) * .035f + UpDown + .023200f), 0.035f, 0.05f, 0, 15, 199, 30, 300);
}
static void DRAW_SPRITE(char* texture, char * name, float X, float Y, float width, float height, float rotation, int r, int g, int b, int a)
{
	GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(texture, 0);
	GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(texture);
	GRAPHICS::DRAW_SPRITE(texture, name, X, Y, width, height, rotation, r, g, b, a);
}
void addOption(char *option, char *info = NULL)
{
	optionCount++;
	if (currentOption == optionCount)
		infoText = info;
	if (currentOption <= maxOptions && optionCount <= maxOptions)
		drawText(option, optionsFont, textXCoord, (optionCount * 0.035f + 0.125f), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
	else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
		drawText(option, optionsFont, textXCoord, ((optionCount - (currentOption - maxOptions)) * 0.035f + 0.125f), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
}
void addSubmenuOption(char *option, int newSubmenu, char *info = NULL)
{
	addOption(option, info);
	if (currentOption == optionCount)
	{
		xInstruction = true;
		if (optionPress)
			changeSubmenu(newSubmenu);
	}
}
void addBoolOption(char *option, bool b00l, char *info = NULL)
{
	char buf[30];
	if (b00l)
	{
		snprintf(buf, sizeof(buf), "%s: On", option);
		addOption(buf, info);
	}
	else
	{
		snprintf(buf, sizeof(buf), "%s: Off", option);
		addOption(buf, info);
	}
}
template<typename TYPE>
void addCustomBoolOption(char *option, TYPE value, TYPE match, char *info = NULL)
{
	char buf[30];
	if (value == match)
	{
		snprintf(buf, sizeof(buf), "%s", option);
		addOption(buf, info);
		DrawSpriteInMenuGreen("commonmenu", "shop_box_tick");
	}
	else
	{
		snprintf(buf, sizeof(buf), "%s", option);
		addOption(buf, info);
		DrawSpriteInMenuRed("commonmenu", "shop_box_cross");
	}
}
void addCheckboxOption(char* option, bool *b00l, char *info = "")
{
	addOption(option, info);
	if (currentOption == optionCount && optionPress) {
		*b00l = !*b00l;
	}
	if (*b00l) {
		DrawSpriteInMenuGreen("commonmenu", "shop_box_tick");
	}
	else {
		DrawSpriteInMenuRed("commonmenu", "shop_box_cross");
	}
}
void addIntOption(char *option, int *var, int min, int max, bool keyboard, char *info = NULL)
{
	char buf[30];
	snprintf(buf, sizeof(buf), "%s: < %i >", option, *var);
	addOption(buf, info);
	if (currentOption == optionCount)
	{
		lrInstruction = true;
		if (rightPress)
		{
			playSound("NAV_UP_DOWN");
			if (*var >= max)
				*var = min;
			else
				*var = *var + 1;
		}
		else if (leftPress)
		{
			playSound("NAV_UP_DOWN");
			if (*var <= min)
				*var = max;
			else
				*var = *var - 1;
		}
		if (keyboard)
		{
			squareInstruction = true;
			if (squarePress)
			{
				startKeyboard(0, "", (strlen(ItoS(max)) + 1));
				keyboardVar = var;
			}
		}
	}
}
void addFloatOption(char *option, float *var, float min, float max, bool holdPress, char *info = NULL)
{
	char buf[30];
	snprintf(buf, sizeof(buf), "%s: < %s >", option, FtoS(*var));
	addOption(buf, info);
	if (currentOption == optionCount)
	{
		lrInstruction = true;
		if (holdPress)
		{
			if (fastRightPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var >= max)
					*var = min;
				else
					*var = *var + 0.01f;
			}
			else if (fastLeftPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var <= min)
					*var = max;
				else
					*var = *var - 0.01f;
			}
		}
		else
		{
			if (rightPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var >= max)
					*var = min;
				else
					*var = *var + 0.01f;
			}
			else if (leftPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var <= min)
					*var = max;
				else
					*var = *var - 0.01f;
			}
		}
	}
}
void addKeyboardOption(char *option, int action, char *defaultText, int maxLength, char *info = NULL)
{
	addOption(option, info);
	if (currentOption == optionCount)
	{
		xInstruction = true;
		if (optionPress)
			startKeyboard(action, defaultText, maxLength);
	}
}
#pragma endregion
int getOption()
{
	xInstruction = true;
	if (optionPress)
		return currentOption;
	else return 0;
}
bool leftsided;

void monitorButtons()
{
	SetupTimer(20);
	if (submenu == Closed)
	{
		if (IsTimerReady())
		{
			if (CONTROLS::IS_CONTROL_PRESSED(0, Dpad_Right) && CONTROLS::IS_CONTROL_PRESSED(0, Button_X))
			{
				menuXCoord = 0.845f;
				submenu = Main_Menu;
				submenuLevel = 0;
				currentOption = 1;
				playSound("YES");
				if (!welcomeShown && PLAYER::IS_PLAYER_ONLINE) {
					char welcomebuf[100];
					snprintf(welcomebuf, sizeof(welcomebuf), "%s Welcome To %s", PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID()), menuName);
					drawNotification(welcomebuf, "Welcome!");
					welcomeShown = true;
				}
			}
			ResetTimer();
		}
	}
	else {
		if (IsTimerReady())
		{
			if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, Button_B))
			{
				if (submenu == Main_Menu)
				{
					submenu = Closed;
				}
				else {
					submenu = lastSubmenu[submenuLevel - 1];
					currentOption = lastOption[submenuLevel - 1];
					submenuLevel--;
				}
				playSound("Back");
			}
			else if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, Button_A))
			{
				optionPress = true;
				playSound("SELECT");
			}
			else if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, Dpad_Up))
			{
				currentOption--;
				if (currentOption < 1)
				{
					currentOption = optionCount;
				}
				playSound("NAV_UP_DOWN");
			}
			else if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, Dpad_Down))
			{
				currentOption++;
				if (currentOption > optionCount)
				{
					currentOption = 1;
				}
				playSound("NAV_UP_DOWN");
			}
			else if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, Dpad_Right))
			{
				rightPress = true;
			}
			else if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, Dpad_Left))
			{
				leftPress = true;
			}
			else if (CONTROLS::IS_CONTROL_PRESSED(0, Dpad_Right))
			{
				fastRightPress = true;
			}
			else if (CONTROLS::IS_CONTROL_PRESSED(0, Dpad_Left))
			{
				fastLeftPress = true;
			}
			else if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, Button_X))
			{
				squarePress = true;
			}
			ResetTimer();
		}
	}
}
void drawTextR(char * text, int font, float x, float y, float scalex, float scaley, int r, int b, int g, int a, int vari = 0, bool haha = false)
{
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(scalex, scaley);
	UI::SET_TEXT_RIGHT_JUSTIFY(true);
	UI::SET_TEXT_WRAP(0.1f, .91f - vari);
	if (!haha) {
		if (currentOption == optionCount)
		{
			UI::SET_TEXT_COLOUR(0, 0, 0, 255);
			UI::_SET_TEXT_ENTRY("STRING");
			UI::_ADD_TEXT_COMPONENT_STRING(text);
			snprintf(opt, sizeof(opt), "%s", text);
		}
		else
		{
			UI::SET_TEXT_COLOUR(r, g, b, a);
			UI::_SET_TEXT_ENTRY("STRING");
			UI::_ADD_TEXT_COMPONENT_STRING(text);
		}
	}
	else {
		UI::SET_TEXT_COLOUR(r, g, b, a);
		UI::_SET_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING(text);
	}
	UI::_DRAW_TEXT(x, y);
}
void DisplayText(const char *text, int font, float x, float y, float scaleX, float scaleY, int r, int g, int b, int a, bool centered)
{
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(scaleX, scaleY);
	UI::SET_TEXT_COLOUR(r, g, b, a);
	UI::SET_TEXT_WRAP(0.0f, 1.0f);
	UI::SET_TEXT_CENTRE(centered);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_OUTLINE();
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DRAW_TEXT(x, y);
}
void DrawCenterText(char* Text, float X, float Y, float S_X, float S_Y, int Font, bool Outline, bool Shadow, bool Center, bool RightJustify, int R, int G, int B, int A)
{
	UI::SET_TEXT_FONT(Font);
	UI::SET_TEXT_SCALE(S_X, S_Y);
	UI::SET_TEXT_COLOUR(R, G, B, A);
	if (Outline)
		UI::SET_TEXT_OUTLINE();
	if (Shadow)
		UI::SET_TEXT_DROP_SHADOW();
	if (Center)
		UI::SET_TEXT_CENTRE(1);
	if (RightJustify)
		UI::SET_TEXT_RIGHT_JUSTIFY(1);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(Text);
	UI::_DRAW_TEXT(X, Y);
}
float PixelX(int Input)
{
	return (float)Input / (float)1280;
}
float PixelY(int Input)
{
	return (float)Input / (float)720;
}
int* showMessage(char* msg) {
	Dialog::msgdialog_mode = Dialog::MODE_STRING_OK;
	Dialog::Show(msg);
}
void normalMenuActions()
{
	UI::HIDE_HELP_TEXT_THIS_FRAME();
	CAM::SET_CINEMATIC_BUTTON_ACTIVE(0);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 19);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 20);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 21);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 27);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 54);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 123);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 124);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 125);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 126);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 138);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 139);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 140);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 177);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 178);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 179);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 207);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 166);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 167);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 177);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 178);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 193);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 194);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 195);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 202);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 203);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 204);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 205);
	if (optionCount > maxOptions)
	{
		GRAPHICS::DRAW_RECT(menuXCoord, (((maxOptions * 0.035f) / 2) + 0.159f), 0.19f, (maxOptions * 0.035f), backgroundRed, backgroundGreen, backgroundBlue, backgroundOpacity);
		if (currentOption > maxOptions)
		{
			GRAPHICS::DRAW_RECT(menuXCoord, 0.156f, 0.19f, 0.005f, indicatorRed, indicatorGreen, indicatorBlue, indicatorOpacity);
		}
		if (currentOption != optionCount)
		{
			GRAPHICS::DRAW_RECT(menuXCoord, ((maxOptions * 0.035f) + 0.161f), 0.19f, 0.005f, indicatorRed, indicatorGreen, indicatorBlue, indicatorOpacity);
		}
	}
	else
	{
		GRAPHICS::DRAW_RECT(menuXCoord, (((optionCount * 0.035f) / 2) + 0.159f), 0.19f, (optionCount * 0.035f), backgroundRed, backgroundGreen, backgroundBlue, backgroundOpacity);
		GRAPHICS::DRAW_RECT(scrollerVar, (((optionCount * 0.035f) / 2) + 0.159f), 0.015f, (optionCount * 0.035f), backgroundRed, backgroundGreen, backgroundBlue, backgroundOpacity);
		GRAPHICS::DRAW_RECT(scrollerVar, ((currentOption * 0.035f) + 0.1415f), 0.015f, 0.035f, bannerRectRed, bannerRectGreen, bannerRectBlue, bannerRectOpacity);
		DRAW_TEXTURE("timerbars", "lhs_bg", scrollerVar, ((currentOption * 0.035f) + 0.1415f), 0.015f, 0.035f, 0, 46, 105, 186, bannerRectOpacity);
		DRAW_TEXTURE("timerbars", "lhs_bg", menuXCoord, (((optionCount + 1) * 0.035f) + 0.1415f), 0.19f, 0.035f, 0, 46, 105, 186, bannerRectOpacity);
		GRAPHICS::DRAW_RECT(menuXCoord, (((optionCount + 1) * 0.035f) + 0.1415f), 0.19f, 0.035f, bannerRectRed, bannerRectGreen, bannerRectBlue, bannerRectOpacity);
		DRAW_TEXTURE("commonmenu", "shop_arrows_upanddown", menuXCoord, (((optionCount + 1) * 0.035f) + 0.1415f), 0.05f, 0.037f, 0, 255, 255, 255, 255);
	}
	if (!GAMEPLAY::IS_STRING_NULL_OR_EMPTY(infoText))
	{
		if (optionCount > maxOptions)
		{
			drawText(infoText, optionsFont, menuXCoord, ((maxOptions + 2) * 0.035f + 0.125f), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, true);
			DRAW_TEXTURE("timerbars", "lhs_bg", menuXCoord, (((maxOptions + 2) * 0.035f) + 0.1415f), 0.19f, 0.035f, 0, 46, 105, 186, bannerRectOpacity);
			GRAPHICS::DRAW_RECT(menuXCoord, (((maxOptions + 2) * 0.035f) + 0.1415f), 0.19f, 0.035f, bannerRectRed, bannerRectGreen, bannerRectBlue, bannerRectOpacity);
		}
		else
		{
			drawText(infoText, optionsFont, menuXCoord, ((optionCount + 2) * 0.035f + 0.125f), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, true);
			DRAW_TEXTURE("timerbars", "lhs_bg", menuXCoord, (((optionCount + 2) * 0.035f) + 0.1415f), 0.19f, 0.035f, 0, 46, 105, 186, bannerRectOpacity);
			GRAPHICS::DRAW_RECT(menuXCoord, (((optionCount + 2) * 0.035f) + 0.1415f), 0.19f, 0.035f, bannerRectRed, bannerRectGreen, bannerRectBlue, bannerRectOpacity);
		}
	}
	SetupTimer2(20);
	if (instructions)
	{
		if (IsTimerReady2())
		{
			if (xInstruction)
				addInstruction(BUTTON_A, "Select");
			if (squareInstruction)
				addInstruction(BUTTON_X, "Keyboard");
			if (lrInstruction)
				addInstruction(BUTTON_DPAD_LEFT_RIGHT, "Scroll");
			addInstruction(BUTTON_DPAD_UP_DOWN, "Scroll");
			addInstruction(BUTTON_B, "Back");
			instructionsClose();
			ResetTimer2();
		}
	}
}
void resetVars()
{
	if (submenu != Closed)
		normalMenuActions();
	optionPress = false;
	rightPress = false;
	leftPress = false;
	fastRightPress = false;
	fastLeftPress = false;
	squarePress = false;
	infoText = NULL;
	instructionsSetupThisFrame = false;
	squareInstruction = false;
	xInstruction = false;
	lrInstruction = false;
}
#pragma endregion

#pragma region Other Loop
void otherLoop()
{
	if (keyboardActive)
	{
		if (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 1)
		{
			keyboardActive = false;
			switch (keyboardAction)
			{
			case 0:
				*keyboardVar = StoI(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT());
				break;
			}
		}
		else if (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 2 || GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 3)
		{
			keyboardActive = false;
		}
	}
	if (movingTitle) {
		if (timerDecrease < (int)GAMEPLAY::GET_FRAME_COUNT())
		{
			timerDecrease = (int)GAMEPLAY::GET_FRAME_COUNT() + 2;
			if (timerTitleDownUp == 0)
			{
				titleScale--;
				if (titleScale <= 0.95f)
				{
					titleScale = 0.95f;
					timerTitleDownUp = 1;
				}
			}
			else if (timerTitleDownUp == 1)
			{
				titleScale++;
				if (titleScale >= 0.90f)
				{
					titleScale = 0.90f;
					timerTitleDownUp = 2;
				}
			}
			else if (timerTitleDownUp == 2)
			{
				titleScale++;
				if (titleScale >= 0.85f)
				{
					titleScale = 0.85f;
					timerTitleDownUp = 3;
				}
			}
			else if (timerTitleDownUp == 3)
			{
				titleScale++;
				if (titleScale >= 0.80f)
				{
					titleScale = 0.80f;
					timerTitleDownUp = 4;
				}
			}
			else if (timerTitleDownUp == 4)
			{
				titleScale++;
				if (titleScale >= 0.75f)
				{
					titleScale = 0.75f;
					timerTitleDownUp = 5;
				}
			}
			else if (timerTitleDownUp == 5)
			{
				titleScale++;
				if (titleScale >= 0.80f)
				{
					titleScale = 0.80f;
					timerTitleDownUp = 6;
				}
			}
			else if (timerTitleDownUp == 6)
			{
				titleScale++;
				if (titleScale >= 0.85f)
				{
					titleScale = 0.85f;
					timerTitleDownUp = 7;
				}
			}
			else if (timerTitleDownUp == 7)
			{
				titleScale++;
				if (titleScale >= 0.90f)
				{
					titleScale = 0.90f;
					timerTitleDownUp = 0;
				}
			}
		}
	}
	if (selfoption[0]) {
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), 1);
		ENTITY::SET_ENTITY_CAN_BE_DAMAGED(PLAYER::PLAYER_PED_ID(), 0);
		ENTITY::SET_ENTITY_PROOFS(PLAYER::PLAYER_PED_ID(), 1, 1, 1, 1, 1, 1, 1, 1);
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), 1);
		PED::SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), 0);
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER::PLAYER_PED_ID(), 0);
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER::PLAYER_PED_ID(), 1);
	}
	else
	{
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), 0);
		ENTITY::SET_ENTITY_CAN_BE_DAMAGED(PLAYER::PLAYER_PED_ID(), 1);
		ENTITY::SET_ENTITY_PROOFS(PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 0, 0);
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), 0);
		PED::SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), 1);
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER::PLAYER_PED_ID(), 1);
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER::PLAYER_PED_ID(), 0);
	}
	if (selfoption[1]) {
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), 0);
	}
	else {
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), 1);
	}
	if (ShowTalkingPlayers)
	{
		char p[100];
		memset(p, 0, sizeof(p));
		char* talker;
		for (int i = 0; i < 19; i++) {
			DisplayText("~r~Talking Players~w~", 1, 0.033, 0.18, 0.590, 0.590, bannerRectRed, bannerRectGreen, bannerRectBlue, bannerRectOpacity, false);
			if (NETWORK::NETWORK_IS_PLAYER_TALKING(i)) {
				talker = PLAYER::GET_PLAYER_NAME(i);
				snprintf(p, 100, "~b~%s", talker);
				drawTalker(p, i);
			}
		}
	}
	if (flashingTitle) {
		int red = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, 255);
		int green = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, 255);
		int blue = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, 255);
		bannerTextRed = red;
		bannerTextGreen = green;
		bannerTextBlue = blue;
	}
}
#pragma endregion

int Hook()
{
	monitorButtons();
	otherLoop();
	optionCount = 0;
	WriteString(0x18205B0, "No Active Player In Slot");
	switch (submenu)
	{
#pragma region Main Menu
	case Main_Menu:
		addTitle(menuName, "MAIN MENU");
		addSubmenuOption("Self Options", Self_Options, "Edit Options For Yourself");
		addSubmenuOption("Players", Players_List, "View A List Of Players");
		addSubmenuOption("All Players", all_player_options, "Do Stuff For All Players");
		addSubmenuOption("Weapon Options", weapon_options, "Modify Your Weapons");
		addSubmenuOption("Teleport Options", teleport_options, "Teleport Your Way Around");
		addSubmenuOption("Messaging Options", all_player_messaging, "Message All Players");
		addSubmenuOption("Vehicle Options", vehicle_options, "Modify Your Vehicle");
		addSubmenuOption("World Options", world_options, "Edit The World");
		addSubmenuOption("Menu Settings", Settings, "Customize The Menu");
		break;
#pragma endregion
#pragma region Players List
	case Players_List:
		addTitle(menuName, "PLAYERS LIST");
		for (int i = 0; i < 18; i++)
		{
			addOption(PLAYER::GET_PLAYER_NAME(i));
		}
		if (optionPress && PLAYER::IS_PLAYER_PLAYING(currentOption - 1))
		{
			selectedPlayer = currentOption - 1;
			changeSubmenu(Player_Options);
		}
		break;
#pragma endregion
#pragma region Player Options
	case Player_Options:
		addTitle(menuName, PLAYER::GET_PLAYER_NAME(selectedPlayer));
		break;
#pragma endregion
#pragma region Self Options
	case Self_Options:
		addTitle(menuName, "SELF OPTIONS");
		addCheckboxOption("Godmode", &selfoption[0]);
		addCheckboxOption("Invisibility", &selfoption[1]);
		switch (getOption())
		{
		case 1:
			if (!selfoption[0])
			{
				drawNotification("Godmode ~r~Disabled", "Godmode");
			}
			else
			{
				drawNotification("Godmode ~g~Enabled", "Godmode");
			}
			break;
		case 2:
			if (!selfoption[1])
			{
				drawNotification("Invisibility ~r~Disabled", "Invisibility");
			}
			else
			{
				drawNotification("Invisibility ~g~Enabled", "Invisibility");
			}
			break;
		}
		break;
#pragma endregion
#pragma region Settings
	case Settings:
		addTitle(menuName, "MENU SETTINGS");
		addSubmenuOption("Banners", Settings_Banners);
		addSubmenuOption("Body", Settings_Body);
		addSubmenuOption("Scroller", Settings_Scroller);
		addSubmenuOption("Indicators", Settings_Indicators);
		addCheckboxOption("Sounds", &menuSounds);
		addCheckboxOption("Instructions", &instructions);
		addCustomBoolOption("Left Sided", menuXCoord, 0.145f);
		addCheckboxOption("Animated Globe", &showGlare);
		addCheckboxOption("Talking Players", &ShowTalkingPlayers);
		addCheckboxOption("Moving Title", &movingTitle);
		addCheckboxOption("Flashing Title", &flashingTitle);
		switch (getOption())
		{
		case 5:
			if (!menuSounds)
			{
				drawNotification("Menu Sounds ~r~Disabled", "Menu Sounds");
			}
			else
			{
				drawNotification("Menu Sounds ~g~Enabled", "Menu Sounds");
			} 
			break;
		case 6:
			if (!instructions)
			{
				drawNotification("Instructions ~r~Disabled", "Instructions");
			}
			else
			{
				drawNotification("Instructions ~g~Enabled", "Instructions");
			}
			break;
		case 7:
			if (menuXCoord == 0.145f)
			{
				menuXCoord = 0.845f;
				textXCoord = 0.76f;
				checkboxshit1 = 0.920;
				scrollerVar = menuXCoord + 0.117f;
				GlareX = 1.10f;
				leftsided = false;
			}
			else
			{
				menuXCoord = 0.145f;
				textXCoord = 0.06f;
				checkboxshit1 = 0.220;
				scrollerVar = menuXCoord - 0.117f;
				GlareX = 0.40;
				leftsided = true;
			}
			break;
		case 8:
			if (!showGlare)
			{
				drawNotification("Animated Globe ~r~Disabled", "Animated Globe");
			}
			else
			{
				drawNotification("Animated Globe ~g~Enabled", "Animated Globe");
			}
			break;
		case 9:
			if (!ShowTalkingPlayers)
			{
				drawNotification("Show Talking Players ~r~Disabled", "Show Talking Players");
			}
			else
			{
				drawNotification("Show Talking Players ~g~Enabled", "Show Talking Players");
			}
			break;
		}
		break;
	case Settings_Banners:
		addTitle(menuName, "BANNER SETTINGS");
		addIntOption("Text Red", &bannerTextRed, 0, 255, true);
		addIntOption("Text Green", &bannerTextGreen, 0, 255, true);
		addIntOption("Text Blue", &bannerTextBlue, 0, 255, true);
		addIntOption("Text Opacity", &bannerTextOpacity, 0, 255, true);
		addIntOption("Text Font", &bannerTextFont, 0, 7, false);
		addIntOption("Rect Red", &bannerRectRed, 0, 255, true);
		addIntOption("Rect Green", &bannerRectGreen, 0, 255, true);
		addIntOption("Rect Blue", &bannerRectBlue, 0, 255, true);
		addIntOption("Rect Opacity", &bannerRectOpacity, 0, 255, true);
		break;
	case Settings_Body:
		addTitle(menuName, "BODY SETTINGS");
		addIntOption("Text Red", &optionsRed, 0, 255, true);
		addIntOption("Text Green", &optionsGreen, 0, 255, true);
		addIntOption("Text Blue", &optionsBlue, 0, 255, true);
		addIntOption("Text Opacity", &optionsOpacity, 0, 255, true);
		addIntOption("Text Font", &optionsFont, 0, 7, false);
		addIntOption("Rect Red", &backgroundRed, 0, 255, true);
		addIntOption("Rect Green", &backgroundGreen, 0, 255, true);
		addIntOption("Rect Blue", &backgroundBlue, 0, 255, true);
		addIntOption("Rect Opacity", &backgroundOpacity, 0, 255, true);
		break;
	case Settings_Scroller:
		addTitle(menuName, "SCROLLER SETTINGS");
		addIntOption("Red", &scrollerRed, 0, 255, true);
		addIntOption("Green", &scrollerGreen, 0, 255, true);
		addIntOption("Blue", &scrollerBlue, 0, 255, true);
		addIntOption("Opacity", &scrollerOpacity, 0, 255, true);
		break;
	case Settings_Indicators:
		addTitle(menuName, "INDICATOR SETTINGS");
		addIntOption("Red", &indicatorRed, 0, 255, true);
		addIntOption("Green", &indicatorGreen, 0, 255, true);
		addIntOption("Blue", &indicatorBlue, 0, 255, true);
		addIntOption("Opacity", &indicatorOpacity, 0, 255, true);
		break;
#pragma endregion
	}
	resetVars();
	return true;
}
void Thread(uint64_t nothing)
{
	sleep(30000);
	g_Natives = (Native_s**)FindNativeTableUIess();
	PatchInJump(NativeUIess(0x9FAB6729, true), (int)Hook, false); 
	sys_ppu_thread_exit(nothing);
}
extern "C" int PRX_ENTRY()
{
	sys_ppu_thread_t ThreadModuleID;
	sys_ppu_thread_create(&ThreadModuleID, Thread, 0, 10, 0x10, 0, menuName);
	char msg[1000];
	snprintf(msg, sizeof(msg), "Welcome To %s Created By 2much4u Revamped By PhucedMODZ", menuName);
	showMessage(msg);
	return SYS_PRX_RESIDENT;
}