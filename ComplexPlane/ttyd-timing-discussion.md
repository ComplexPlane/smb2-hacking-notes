@PistonMiner would you be able to look into how the game's time is handled sometime? I wanna have an option for a timer that doesnt pause during loading zones, but im having a hard time figuring out exactly how to handle it
 
PistonMinerLast Friday at 5:10 PM
You'll have to be more specific than "time"
If you want real time, use OSGetTime
 
ZephilesLast Friday at 5:10 PM
ya, i need to use that
but the game does other stuff to keep it accurate, like saving some variables in save files
 
PistonMinerLast Friday at 5:11 PM
"Keep it accurate"?
The time base registers are gonna be the most accurate for real world time
 
ZephilesLast Friday at 5:12 PM
like, if you save and then reload the file minutes later, the time displayed in the game will still be correct
 
PistonMinerLast Friday at 5:12 PM
"Time displayed in the game" being what
 
ZephilesLast Friday at 5:12 PM
being the timer in the pause menu
 
PistonMinerLast Friday at 5:13 PM
So you don't want wall time but file played time
 
ZephilesLast Friday at 5:13 PM
yes but it needs to be its own variable
so it can be paused/reset/etc
 
PistonMinerLast Friday at 5:14 PM
And do you need it to be saved to the file?
 
ZephilesLast Friday at 5:14 PM
no
 
PistonMinerLast Friday at 5:14 PM
So then what does the file time matter
 
ZephilesLast Friday at 5:15 PM
i want a timer that functions the same as the one in the pause menu
so i need to know how it works, so i can recreate it
 
PistonMinerLast Friday at 5:17 PM
I mean I'm pretty sure that comes directly from OSGetTime
 
ZephilesLast Friday at 5:17 PM
yes but it does other stuff with it
 
PistonMinerLast Friday at 5:17 PM
Or the VI callback if that supplies something special
I mean not really IIRC
It subtracts it from last frame's time, thus deriving the frame duration, and then if the timer is enabled it's added
 
ZephilesLast Friday at 5:19 PM
i'll try that out and see if it works properly
 
PistonMinerLast Friday at 5:21 PM
There's probably a global for last frame duration you can just use too
 
ZephilesLast Friday at 5:21 PM
im reasonably certain that thats a variable in the globalptr struct, but idk which one it is
is it at 0x30?
or maybe im thinking of the wrong thing
 
PistonMinerLast Friday at 5:41 PM
I think 0x28 is last frame duration
Yeah, it's +0x28, at least on US.
And the logic that actually does the updates of those different internal timers is all in viPostCallback
 
ZephilesLast Friday at 5:44 PM
ya im seeing all the logic, im just having a hard time putting it all together
and Ghidra doesnt help much with it
 
PistonMinerLast Friday at 5:44 PM
What is it you're having trouble with precisely?
 
ZephilesLast Friday at 5:45 PM
keeping track of all the math
 
PistonMinerLast Friday at 5:45 PM
There's exactly one mathematical operation here
If you want your timer to run, you add the field at +0x28
If not, you don't
And that add is a normal 64-bit add
 
ZephilesLast Friday at 5:46 PM
ya, but viPostCallback is doing a bunch of checks and whatnot with these variables, as well as a bunch of other math
 
PistonMinerLast Friday at 5:46 PM
viPostCallback does more than just handling those timers
It also derives that actual time between frame etc
You don't need to do all that
Like, e.g. the block @ 0x8006f578 US is all you need to do
Literally just an if and an add
(modify-add I guess)
 
ZephilesLast Friday at 5:57 PM
that does work, but this has the exact same problem as before: it doesnt update during loading zones
 
ZephilesLast Friday at 6:11 PM
@PistonMiner manually setting the previous frame time and then doing the subtracting works, but im comparing that timer to my previous one, and this new one desynchs, as in gets slightly ahead of my previous timer. Is this due to lag and/or skipped frames?
i'll make a recording
top timer is the new one
 
PistonMinerLast Friday at 6:17 PM
And bottom timer is what?
 
ZephilesLast Friday at 6:17 PM
the one thats in the current release
 
PistonMinerLast Friday at 6:17 PM
Which works how?
 
ZephilesLast Friday at 6:18 PM
increments by 1 every time the draw function is ran
 
PistonMinerLast Friday at 6:18 PM
I mean yeah that's bound to be inaccurate
 
ZephilesLast Friday at 6:18 PM
:thinking:
 
PistonMinerLast Friday at 6:18 PM
Frames aren't exactly 16.67ms on the dot
 
ZephilesLast Friday at 6:18 PM
so the top timer is more accurate then
 
PistonMinerLast Friday at 6:18 PM
That's a frame counter
You wanted wall time
If the game runs at 5 FPS then OSGetTime will still run full speed
 
ZephilesLast Friday at 6:20 PM
ok, so how do i make a frame counter that doesnt pause during loads :thinking:
 
PistonMinerLast Friday at 6:21 PM
That's contradictory
 
ZephilesLast Friday at 6:21 PM
:NotLikeThis~1:
 
PistonMinerLast Friday at 6:21 PM
I mean if there are no frames then there's nothing to count
Do you actually want a frame counter or a timer
Because for a timer, like I said, use OSGetTime
Or the whole viPostCallback shebang
They do that in order to get the time neatly for the next frame they should simulate for
Like that's a matter of what time you want to see on screen: The one at the start of the frame? The one when the draw order was made?
Any?
Because if you just want any you can ignore the viPostCallback stuff and just roll your own
The timer should restart fine though, even if a retrace was some time ago
What code did you actually write and what is the actual behavior you're seeing when loading?
 
ZephilesLast Friday at 6:26 PM
i set up a variable for the previous frame time that is updated after the draw, and the increment amount is determined by subtracting the previous frame time from the current frame time
 
PistonMinerLast Friday at 6:26 PM
Code please
 
ZephilesLast Friday at 6:28 PM
int64_t CurrentTime = gc::OSTime::OSGetTime();
int64_t IncrementAmount = CurrentTime - OnScreenTimer.PreviousFrameTime;
OnScreenTimer.MainTimer += IncrementAmount;
OnScreenTimer.PreviousFrameTime = CurrentTime;
 
PistonMinerLast Friday at 6:28 PM
And you're calling this when?
 
ZephilesLast Friday at 6:29 PM
in the drawing function
 
PistonMinerLast Friday at 6:29 PM
I mean that should give you some time that should be accurate to +- one frame
What's the problem?
 
ZephilesLast Friday at 6:29 PM
the problem appears to be the FPS
 
PistonMinerLast Friday at 6:30 PM
This is completely invariant to FPS
If the game runs at 5 FPS this will still run full speed
And it will run through loads or stalls or whatever
 
ZephilesLast Friday at 6:30 PM
well apparently thats not true, because the timers desync very early on in central Rogueport
 
PistonMinerLast Friday at 6:31 PM
Timers plural means what?
Your frame counter vs this one?
 
ZephilesLast Friday at 6:31 PM
yes
 
PistonMinerLast Friday at 6:31 PM
I mean
Yeah
Counting frames and multiplying that by 16.67ms is not an accurate way to measure walltime
 
ZephilesLast Friday at 6:31 PM
but the frames are more accurate for testing and whatnot, especially if people are timing stuff in Dolphin
 
PistonMinerLast Friday at 6:32 PM
Well I mean you can't have your cake and eat it too
 
ZephilesLast Friday at 6:32 PM
but it becomes inaccurate when it pauses during loads
 
PistonMinerLast Friday at 6:32 PM
You can either have the real time or the frame count
 
ZephilesLast Friday at 6:32 PM
:NotLikeThis~1:
 
PistonMinerLast Friday at 6:33 PM
I don't know what you're asking right now, it's literally contradictory
 
ZephilesLast Friday at 6:34 PM
@Vynneve i think you might wanna input your definition of what the timer should do
im not happy with either, so im probably just gonna make a separate display for a real-time timer, and change the name of the current one to frame counter
 
PistonMinerLast Friday at 6:36 PM
Define for me what properties you want this timer of yours to have
 
ZephilesLast Friday at 6:37 PM
I need to run an errand, so I will when I get back
 
ZephilesLast Friday at 6:49 PM
The ideal timer would be the frame counter, but not pausing during loads
Pausing during loads is literally the only problem with it
 
PistonMinerLast Friday at 6:52 PM
Does the game have frames during load?
Because if we're going there we gotta be real precise about what a "frame" is in this definition
 
ZephilesLast Friday at 6:53 PM
Why wouldn't it?
 
PistonMinerLast Friday at 6:54 PM
You know what, use the retrace counter
 
ZephilesLast Friday at 6:54 PM
How is that handled?
 
PistonMinerLast Friday at 6:54 PM
[gpGlobals+0x1c]
32-bit integer
 
ZephilesLast Friday at 6:55 PM
How does it work? Like, what determines if it should increment, etc
 
PistonMinerLast Friday at 6:58 PM
Every time VI posts a retrace, that'll update.
That's as always-framey as you can get.
Now notice if you miss a retrace, so if a game tick takes two frames, I'm pretty sure that's still gonna update
(And in fact I don't know if there's race conditions with that...?)
 
ZephilesLast Friday at 7:01 PM
Alright, I'll mess around with it when I get back to my PC
Is it signed or unsigned?
 
PistonMinerLast Friday at 7:02 PM
Unsigned I would guess
 
VynneveLast Friday at 7:11 PM
i remember dolphin can pause it's frame count during loads. so i guess it doesnt? i don't really understand it though
 
ZephilesLast Friday at 7:44 PM
:thinking: using the retrace counter gives me almost the same result as OSTime
 
PistonMinerLast Friday at 7:48 PM
That's actual real VI frames.
So any drift you see between that and your manual frame counter is going to be missed frames.
 
ZephilesLast Friday at 7:50 PM
well then this doesnt do what i want either
 
ZephilesLast Friday at 8:04 PM
i think im just gonna have to go with OSTime, since that would be the equivalent to LiveSplit or some other external timer
 
PistonMinerLast Friday at 8:13 PM
well then this doesnt do what i want either
Well then I'm pretty sure there is no concept of time in this game that maps to what you are thinking of.
AFAIK the entire time spent loading is a missed frame.
And you don't want to account for those.
So your concept is as best I can tell quite simply contradictory.
You can do something like switch to VI frames while loading and then switch back to game frames but that's entirely arbitrary.
 
GitHubBOTYesterday at 2:38 AM

Zephiles
[TTYD-Practice-Codes:master] 2 new commits
2c56584 Removed unnecessary bool check in memorywatch.cpp - Zephiles
7fa8f78 Adjusted the On-Screen Timer code, and added a ... - Zephiles
 
ZephilesYesterday at 2:43 AM
i'll probably put up a release tomorrow if nothing needs to be fixed/adjusted
@Vynneve you can try it out and see if it works as it should. If you have both the on-screen timer and the frame counter on at the same time, the frame counter will be above the on-screen timer.
Attachment file type: unknown
rel.jp.gci
128.06 KB
 
VynneveYesterday at 3:01 AM
Can't atm. Might have some time TMR
 
ZephilesYesterday at 3:10 AM
if you do get a chance to test it tomorrow, then you should probably check that it runs correctly in Nintendont, as time is handled differently in it
 
GitHubBOTYesterday at 2:44 PM

Zephiles
[TTYD-Practice-Codes:master] 1 new commit
bea7acf For the Lock Flags struct, created another stru... - Zephiles
 
ZephilesYesterday at 11:41 PM
Gengar tested it for me, so you dont need to anymore Vynneve
probably gonna put up the release in a bit
 
GitHubBOTToday at 12:05 AM

Zephiles
[TTYD-Practice-Codes:master] 1 new commit
c83de32 Update USER_MANUAL.md - Zephiles

Zephiles
[Zephiles/TTYD-Practice-Codes] New release published: v3.0.41
 
ZephilesToday at 12:11 AM
@Vynneve ^ now you have a proper on-screen timer
 
VynneveToday at 12:15 AM
sweet
