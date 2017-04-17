/*
================================================================================

Copyright (c) 2012, Dee E. Abbott
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
* Neither the name of the organization nor the names of its contributors may
be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

================================================================================
*/

#include "mylo.hpp"

platform::input_interface::metadata platform::input_interface::metadata_buttons[20] = {
  { "", 0x00,	"" },
  { "VK_LBUTTON", 0x01,	"Left mouse button" },
  { "RI_MOUSE_LEFT_BUTTON_DOWN", 0x0001, "Left button changed to down." },
  { "RI_MOUSE_LEFT_BUTTON_UP", 0x0002, "Left button changed to up." },
  { "RI_MOUSE_MIDDLE_BUTTON_DOWN", 0x0010, "Middle button changed to down." },
  { "RI_MOUSE_MIDDLE_BUTTON_UP", 0x0020, "Middle button changed to up." },
  { "RI_MOUSE_RIGHT_BUTTON_DOWN", 0x0004, "Right button changed to down." },
  { "RI_MOUSE_RIGHT_BUTTON_UP", 0x0008, "Right button changed to up." },
  { "RI_MOUSE_BUTTON_1_DOWN", 0x0001, "RI_MOUSE_LEFT_BUTTON_DOWN" },
  { "RI_MOUSE_BUTTON_1_UP", 0x0002, "RI_MOUSE_LEFT_BUTTON_UP" },
  { "RI_MOUSE_BUTTON_2_DOWN", 0x0004, "RI_MOUSE_RIGHT_BUTTON_DOWN" },
  { "RI_MOUSE_BUTTON_2_UP", 0x0008, "RI_MOUSE_RIGHT_BUTTON_UP" },
  { "RI_MOUSE_BUTTON_3_DOWN", 0x0010, "RI_MOUSE_MIDDLE_BUTTON_DOWN" },
  { "RI_MOUSE_BUTTON_3_UP", 0x0020, "RI_MOUSE_MIDDLE_BUTTON_UP" },
  { "RI_MOUSE_BUTTON_4_DOWN", 0x0040, "XBUTTON1 changed to down." },
  { "RI_MOUSE_BUTTON_4_UP", 0x0080, "XBUTTON1 changed to up." },
  { "RI_MOUSE_BUTTON_5_DOWN", 0x100, "XBUTTON2 changed to down." },
  { "RI_MOUSE_BUTTON_5_UP", 0x0200, "XBUTTON2 changed to up." },
  { "RI_MOUSE_WHEEL", 0x0400, "Raw input comes from a mouse wheel. The wheel delta is stored in usButtonData." }
};

// https://msdn.microsoft.com/en-us/library/ms927178.aspx

platform::input_interface::metadata platform::input_interface::metadata_keys[256] = {
  { "", 0x00,	"" },
  { "VK_LBUTTON", 0x01,	"Left mouse button" },
  { "VK_RBUTTON", 0x02,	"Right mouse button" },
  { "VK_CANCEL", 0x03,	"Control-break processing" },
  { "VK_MBUTTON", 0x04,	"Middle mouse button on a three-button mouse" },
  { "", 0x05, "Undefined" },
  { "", 0x06, "Undefined" },
  { "", 0x07,	"Undefined" },
  { "VK_BACK", 	0x08,	"BACKSPACE key" },
  { "VK_TAB", 0x09,	"TAB key" },
  { "", 0x0A, "Undefined" },
  { "", 0x0B,	"Undefined" },
  { "VK_CLEAR", 0x0C,	"CLEAR key" },
  { "VK_RETURN", 0x0D,	"ENTER key" },
  { "", 0x0E, "Undefined" },
  { "", 0x0F,	"Undefined" },
  { "VK_SHIFT", 0x10,	"SHIFT key" },
  { "VK_CONTROL", 0x11,	"CTRL key" },
  { "VK_MENU", 	0x12,	"ALT key" },
  { "VK_PAUSE", 0x13,	"PAUSE key" },
  { "VK_CAPITAL", 0x14,	"CAPS LOCK key" },
  { "", 0x15, "Undefined" },
  { "", 0x16, "Undefined" },
  { "", 0x17, "Undefined" },
  { "", 0x18, "Undefined" },
  { "", 0x19,	"Reserved for Kanji systems" },
  { "", 0x1A,	"Undefined" },
  { "VK_ESCAPE", 0x1B,	"ESC key" },
  { "", 0x1C, "Undefined" },
  { "", 0x1D, "Undefined" },
  { "", 0x1E, "Undefined" },
  { "", 0x1F,	"Reserved for Kanji systems" },
  { "VK_SPACE", 0x20,	"SPACEBAR" },
  { "VK_PRIOR", 0x21,	"PAGE UP key" },
  { "VK_NEXT", 	0x22,	"PAGE DOWN key" },
  { "VK_END", 0x23,	"END key" },
  { "VK_HOME", 	0x24,	"HOME key" },
  { "VK_LEFT", 	0x25,	"LEFT ARROW key" },
  { "VK_UP", 0x26,	"UP ARROW key" },
  { "VK_RIGHT", 0x27,	"RIGHT ARROW key" },
  { "VK_DOWN", 	0x28,	"DOWN ARROW key" },
  { "VK_SELECT", 0x29,	"SELECT key" },
  { "", 0x2A,	"Specific to original equipment manufacturer" },
  { "VK_EXECUTE", 0x2B,	"EXECUTE key" },
  { "VK_SNAPSHOT", 	0x2C,	"PRINT SCREEN key" },
  { "VK_INSERT", 0x2D,	"INS key" },
  { "VK_DELETE", 0x2E,	"DEL key" },
  { "VK_HELP", 	0x2F,	"HELP key" },
  { "", 0x30,    "Undefined" },
  { "", 0x31,    "Undefined" },
  { "", 0x32,    "Undefined" },
  { "", 0x33,    "Undefined" },
  { "", 0x34,    "Undefined" },
  { "", 0x35,    "Undefined" },
  { "", 0x36,    "Undefined" },
  { "", 0x37,    "Undefined" },
  { "", 0x38,    "Undefined" },
  { "", 0x39,    "Undefined" },
  { "", 0x3A,    "Undefined" },
  { "", 0x3B,    "Undefined" },
  { "", 0x3C,    "Undefined" },
  { "", 0x3D,    "Undefined" },
  { "", 0x3E,    "Undefined" },
  { "", 0x3F,    "Undefined" },
  { "", 0x40,	"Undefined" },
  { "", 0x41,    "Undefined" },
  { "", 0x42,    "Undefined" },
  { "", 0x43,    "Undefined" },
  { "", 0x44,    "Undefined" },
  { "", 0x45,    "Undefined" },
  { "", 0x46,    "Undefined" },
  { "", 0x47,    "Undefined" },
  { "", 0x48,    "Undefined" },
  { "", 0x49,    "Undefined" },
  { "", 0x4A,    "Undefined" },
  { "", 0x4B,    "Undefined" },
  { "", 0x4C,    "Undefined" },
  { "", 0x4D,    "Undefined" },
  { "", 0x4E,    "Undefined" },
  { "", 0x4F,    "Undefined" },
  { "", 0x50,    "Undefined" },
  { "", 0x51,    "Undefined" },
  { "", 0x52,    "Undefined" },
  { "", 0x53,    "Undefined" },
  { "", 0x54,    "Undefined" },
  { "", 0x55,    "Undefined" },
  { "", 0x56,    "Undefined" },
  { "", 0x57,    "Undefined" },
  { "", 0x58,    "Undefined" },
  { "", 0x59,    "Undefined" },
  { "", 0x5A,    "Undefined" },
  { "VK_LWIN", 	0x5B,	"Left Windows key on a Microsoft Natural Keyboard" },
  { "VK_RWIN", 	0x5C,	"Right Windows key on a Microsoft Natural Keyboard" },
  { "VK_APPS", 	0x5D,	"Applications key on a Microsoft Natural Keyboard" },
  { "", 0x5E,    "" },
  { "", 0x5F,	"Undefined" },
  { "VK_NUMPAD0", 0x60,	"Numeric keypad 0 key" },
  { "VK_NUMPAD1", 0x61,	"Numeric keypad 1 key" },
  { "VK_NUMPAD2", 0x62,	"Numeric keypad 2 key" },
  { "VK_NUMPAD3", 0x63,	"Numeric keypad 3 key" },
  { "VK_NUMPAD4", 0x64,	"Numeric keypad 4 key" },
  { "VK_NUMPAD5", 0x65,	"Numeric keypad 5 key" },
  { "VK_NUMPAD6", 0x66,	"Numeric keypad 6 key" },
  { "VK_NUMPAD7", 0x67,	"Numeric keypad 7 key" },
  { "VK_NUMPAD8", 0x68,	"Numeric keypad 8 key" },
  { "VK_NUMPAD9", 0x69,	"Numeric keypad 9 key" },
  { "VK_MULTIPLY", 	0x6A,	"Multiply key" },
  { "VK_ADD", 0x6B,	"Add key" },
  { "VK_SEPARATOR", 0x6C,	"Separator key" },
  { "VK_SUBTRACT", 	0x6D,	"Subtract key" },
  { "VK_DECIMAL", 0x6E,	"Decimal key" },
  { "VK_DIVIDE", 0x6F,	"Divide key" },
  { "VK_F1", 0x70,	"F1 key" },
  { "VK_F2", 0x71,	"F2 key" },
  { "VK_F3", 0x72,	"F3 key" },
  { "VK_F4", 0x73,	"F4 key" },
  { "VK_F5", 0x74,	"F5 key" },
  { "VK_F6", 0x75,	"F6 key" },
  { "VK_F7", 0x76,	"F7 key" },
  { "VK_F8", 0x77,	"F8 key" },
  { "VK_F9", 0x78,	"F9 key" },
  { "VK_F10", 0x79,	"F10 key" },
  { "VK_F11", 0x7A,	"F11 key" },
  { "VK_F12", 0x7B,	"F12 key" },
  { "VK_F13", 0x7C,	"F13 key" },
  { "VK_F14", 0x7D,	"F14 key" },
  { "VK_F15", 0x7E,	"F15 key" },
  { "VK_F16", 0x7F,	"F16 key" },
  { "VK_F17", 0x80,	"F17 key" },
  { "VK_F18", 0x81,	"F18 key" },
  { "VK_F19", 0x82,	"F19 key" },
  { "VK_F20", 0x83,	"F20 key" },
  { "VK_F21", 0x84,	"F21 key" },
  { "VK_F22", 0x85,	"F22 key (PPC only) Key used to lock device." },
  { "VK_F23", 0x86,	"F23 key" },
  { "VK_F24", 0x87,	"F24 key" },
  { "", 0x88,    "Undefined" },
  { "", 0x89,    "Undefined" },
  { "", 0x8A,    "Undefined" },
  { "", 0x8B,    "Undefined" },
  { "", 0x8C,    "Undefined" },
  { "", 0x8D,    "Undefined" },
  { "", 0x8E,    "Undefined" },
  { "", 0x8F,	"Undefined" },
  { "VK_NUMLOCK", 0x90,	"NUM LOCK key" },
  { "VK_SCROLL", 0x91,	"SCROLL LOCK key" },
  { "VK_LSHIFT", 0xA0,	"Left SHIFT" },
  { "VK_RSHIFT", 0xA1,	"Right SHIFT" },
  { "VK_LCONTROL", 	0xA2,	"Left CTRL" },
  { "VK_RCONTROL", 	0xA3,	"Right CTRL" },
  { "VK_LMENU", 0xA4,	"Left ALT" },
  { "VK_RMENU", 0xA5,	"Right ALT" },
  { "", 0xA6,    "Undefined" },
  { "", 0xA7,    "Undefined" },
  { "", 0xA8,    "Undefined" },
  { "", 0xA9,    "Undefined" },
  { "", 0xAA,    "Undefined" },
  { "", 0xAB,    "Undefined" },
  { "", 0xAC,    "Undefined" },
  { "", 0xAD,    "Undefined" },
  { "", 0xAE,    "Undefined" },
  { "", 0xAF,    "Undefined" },
  { "", 0xB0,    "Undefined" },
  { "", 0xB1,    "Undefined" },
  { "", 0xB2,    "Undefined" },
  { "", 0xB3,    "Undefined" },
  { "", 0xB4,    "Undefined" },
  { "", 0xB5,    "Undefined" },
  { "", 0xB6,    "Undefined" },
  { "", 0xB7,    "Undefined" },
  { "", 0xB8,    "Undefined" },
  { "", 0xB9,    "Undefined" },
  { "", 0xBA,	"Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xBB,    "Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xBC,    "Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xBD,    "Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xBE,    "Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xBF,    "Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xC0,	"Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xC1,    "Undefined" },
  { "", 0xC2,    "Undefined" },
  { "", 0xC3,    "Undefined" },
  { "", 0xC4,    "Undefined" },
  { "", 0xC5,    "Undefined" },
  { "", 0xC6,    "Undefined" },
  { "", 0xC7,    "Undefined" },
  { "", 0xC8,    "Undefined" },
  { "", 0xC9,    "Undefined" },
  { "", 0xCA,    "Undefined" },
  { "", 0xCB,    "Undefined" },
  { "", 0xCC,    "Undefined" },
  { "", 0xCD,    "Undefined" },
  { "", 0xCE,    "Undefined" },
  { "", 0xCF,    "Undefined" },
  { "", 0xD0,    "Undefined" },
  { "", 0xD1,    "Undefined" },
  { "", 0xD2,    "Undefined" },
  { "", 0xD3,    "Undefined" },
  { "", 0xD4,    "Undefined" },
  { "", 0xD5,    "Undefined" },
  { "", 0xD6,    "Undefined" },
  { "", 0xD7,    "Undefined" },
  { "", 0xD8,    "Undefined" },
  { "", 0xD9,    "Undefined" },
  { "", 0xD0,    "Undefined" },
  { "", 0xD1,    "Undefined" },
  { "", 0xD2,    "Undefined" },
  { "", 0xD3,    "Undefined" },
  { "", 0xD4,    "Undefined" },
  { "", 0xD5,    "Undefined" },
  { "", 0xD6,    "Undefined" },
  { "", 0xD7,    "Undefined" },
  { "", 0xD8,    "Undefined" },
  { "", 0xD9,    "Undefined" },
  { "", 0xDA,	"Undefined" },
  { "", 0xDB,	"Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xDC,    "Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xDD,    "Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xDE,    "Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xDF,    "Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xE0,    "Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xE1,	"Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xE2,	"Specific to original equipment manufacturer; reserved. See following tables." },
  { "", 0xE3,	"Specific to original equipment manufacturer" },
  { "", 0xE4,	"Specific to original equipment manufacturer" },
  { "", 0xE5,	"Undefined" },
  { "", 0xE6,	"Specific to original equipment manufacturer" },
  { "VK_PACKET", 0xE7,	"Used to pass Unicode characters as if they were keystrokes. If VK_PACKET is used with SendInput, then the Unicode character to be delivered should be placed into the lower 16 bits of the scan code. If a keyboard message is removed from the message queue and the virtual key is VK_PACKET, then the Unicode character will be the upper 16 bits of the lparam." },
  { "", 0xE8,	"Undefined" },
  { "", 0xE9,    "Undefined" },
  { "", 0xEA,    "Undefined" },
  { "", 0xEB,    "Undefined" },
  { "", 0xEC,    "Undefined" },
  { "", 0xED,    "Undefined" },
  { "", 0xEE,    "Undefined" },
  { "", 0xEF,    "Undefined" },
  { "", 0xF0,    "Undefined" },
  { "", 0xF1,    "Undefined" },
  { "", 0xF2,    "Undefined" },
  { "", 0xF3,    "Undefined" },
  { "", 0xF4,    "Undefined" },
  { "", 0xF5,	"Specific to original equipment manufacturer" },
  { "VK_ATTN", 	0xF6,	"ATTN key" },
  { "VK_CRSEL", 0xF7,	"CRSEL key" },
  { "VK_EXSEL", 0xF8,	"EXSEL key" },
  { "VK_EREOF", 0xF9,	"Erase EOF key" },
  { "VK_PLAY", 	0xFA,	"PLAY key" },
  { "VK_ZOOM", 	0xFB,	"ZOOM key" },
  { "VK_NONAME", 0xFC,	"Reserved for future use" },
  { "VK_PA1", 0xFD,	"PA1 key" },
  { "VK_OEM_CLEAR", 0xFE,	"CLEAR key" }
};


  /*

  Unaccounted for

  VK_KEYLOCK	F22	Key used to lock device
  Original equipment manufacturers should make special note of the VK key ranges reserved for specific original equipment manufacturer use: 2A, DBE4, E6, and E9F5.
  In addition to the VK key assignments in the previous table, Microsoft has assigned the following specific original equipment manufacturer VK keys.
  Symbolic constant	Hexadecimal value	Mouse or keyboard equivalent

  VK_OEM_SCROLL	0x91	None
  VK_OEM_1	0xBA	";:" for US
  VK_OEM_PLUS	0xBB	"+" any country/region
  VK_OEM_COMMA	0xBC	"," any country/region
  VK_OEM_MINUS	0xBD	"-" any country/region
  VK_OEM_PERIOD	0xBE	"." any country/region
  VK_OEM_2	0xBF	"/?" for US
  VK_OEM_3	0xC0	"`~" for US
  VK_OEM_4	0xDB	"[{" for US
  VK_OEM_5	0xDC	"\|" for US
  VK_OEM_6	0xDD	"]}" for US
  VK_OEM_7	0xDE	"'"" for US
  VK_OEM_8	0xDF	None
  VK_OEM_AX	0xE1	AX key on Japanese AX keyboard
  VK_OEM_102	0xE2	"<>" or "\|" on RT 102-key keyboard
  For East Asian Input Method Editors (IMEs) the following additional virtual keyboard definitions must be observed.
  Symbolic constant	Hexadecimal value	Description
  VK_DBE_ALPHANUMERIC	0x0f0	Changes the mode to alphanumeric.
  VK_DBE_KATAKANA	0x0f1	Changes the mode to Katakana.
  VK_DBE_HIRAGANA	0x0f2	Changes the mode to Hiragana.
  VK_DBE_SBCSCHAR	0x0f3	Changes the mode to single-byte characters.
  VK_DBE_DBCSCHAR	0x0f4	Changes the mode to double-byte characters.
  VK_DBE_ROMAN	0x0f5	Changes the mode to Roman characters.
  VK_DBE_NOROMAN	0x0f6	Changes the mode to non-Roman characters.
  VK_DBE_ENTERWORDREGISTERMODE	0x0f7	Activates the word registration dialog box.
  VK_DBE_ENTERIMECONFIGMODE	0x0f8	Activates a dialog box for setting up an IME environment.
  VK_DBE_FLUSHSTRING	0x0f9	Deletes the undetermined string without determining it.
  VK_DBE_CODEINPUT	0x0fa	Changes the mode to code input.
  VK_DBE_NOCODEINPUT	0x0fb	Changes the mode to no-code input.

  */






