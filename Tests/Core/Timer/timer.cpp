/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

static int g_TimerValue1 = 0;
static int g_TimerValue2 = 0;
static int g_TimerValue3 = 0;
static int g_TimerValue4 = 0;
static int g_TimerValue5 = 0;
static int g_TimerValue6 = 0;

void TestApp::test_timer(void)
{
	Console::write_line(" Header: timer.h");
	Console::write_line("  Class: Timer");

	Console::write_line("   Function: start() (for 1600ms)");
	
	g_TimerValue1 = 0;
	g_TimerValue2 = 0;
	g_TimerValue3 = 0;
	g_TimerValue4 = 0;
	g_TimerValue5 = 0;
	g_TimerValue6 = 0;

	Timer timer_1;
	Timer timer_2;
	Timer timer_3;
	Timer timer_4;
	Timer timer_5;
	Timer timer_6;

	uint64_t start_time = System::get_time();

	timer_1.func_expired() = bind_member(this, &TestApp::funx_timer_1);
	timer_2.func_expired() = bind_member(this, &TestApp::funx_timer_2);
	timer_3.func_expired() = bind_member(this, &TestApp::funx_timer_3);
	timer_4.func_expired() = bind_member(this, &TestApp::funx_timer_4);
	timer_5.func_expired() = bind_member(this, &TestApp::funx_timer_5);
	timer_6.func_expired() = bind_member(this, &TestApp::funx_timer_6);

	timer_1.start(500, false);
	timer_2.start(1000, false);
	timer_3.start(1500, false);
	timer_4.start(500, true);
	timer_5.start(1000, true);
	timer_6.start(1500, true);

	bool stopped_flag = false;
	while(true)
	{
		uint64_t time_now = System::get_time();
		int time_diff = time_now - start_time;
		if (time_diff >= 1600) break;

		// Stop timer after 1200ms
		if (time_diff >= 1200)
		{
				if (!stopped_flag)
				{
						timer_3.stop();
						timer_4.stop();
						stopped_flag = true;
				}
		}

		RunLoop::process();
	}

	if (g_TimerValue1 != 1) fail();
	if (g_TimerValue2 != 1) fail();
	if (g_TimerValue3 != 0) fail();
	if (g_TimerValue4 != 2) fail();
	if (g_TimerValue5 != 1) fail();
	if (g_TimerValue6 != 1) fail();


	Console::write_line("   Function: start() Test adjusting timer after started");

	stopped_flag = false;
	start_time = System::get_time();
	timer_1.stop();
	g_TimerValue1 = 0;
	timer_1.start(3000, false);
	while(true)
	{
		uint64_t time_now = System::get_time();
		int time_diff = time_now - start_time;
		if (time_diff >= 1600)
			break;

		// Reset timer after 500ms
		if (time_diff >= 500)
		{
				if (!stopped_flag)
				{
						timer_1.start(100, false);
						stopped_flag = true;
				}
		}

		RunLoop::process();
	}
	if (g_TimerValue1 != 1) fail();

}

void TestApp::funx_timer_1()
{
	g_TimerValue1++;
}
void TestApp::funx_timer_2()
{
	g_TimerValue2++;
}
void TestApp::funx_timer_3()
{
	g_TimerValue3++;
}
void TestApp::funx_timer_4()
{
	g_TimerValue4++;
}
void TestApp::funx_timer_5()
{
	g_TimerValue5++;
}
void TestApp::funx_timer_6()
{
	g_TimerValue6++;
}
