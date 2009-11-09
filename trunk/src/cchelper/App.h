#pragma once

// DEFINES
//___________________________________________________________________________s

#define MAX_LOADSTRING	256

#define THROW_CHECK(hr,s) if (hr != S_OK) throw(s);

// STRUCTS
//___________________________________________________________________________


// DECLARATIONS OF FUNCTIONS
//___________________________________________________________________________
BOOL InitApp();
BOOL AppLoop();
BOOL ExitApp();

void SetAlarm();
void KillAlarm();
