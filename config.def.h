/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */

static const char *fonts[]          = {
    "Consolas:pixelsize=14:antialias=true:autohint=true",
    "Font Awesome 6 Free:style=solid,regular:pixelsize=12:antialias=true:autohint=true",
    "Font Awesome 6 Brands:style=solid,regular:pixelsize=12:antialias=true:autohint=true",
    "Symbols Nerd Font:pixelsize=12:antialias=true:autohint=true",
};
static const char dmenufont[] = "Consolas:pixelsize=14:antialias=true:autohint=true";

/* 8 normal colors
"#000000", black  
"#ff5555", red    
"#50fa7b", green  
"#f1fa8c", yellow 
"#bd93f9", blue   
"#ff79c6", magenta
"#8be9fd", cyan   
"#bbbbbb", white */
/* 8 bright colors
"#44475a", black  
"#ff5555", red    
"#50fa7b", green  
"#f1fa8c", yellow 
"#bd93f9", blue   
"#ff79c6", magenta
"#8be9fd", cyan   
"#ffffff", white */
static const char col_gray1[]  = "#222222";
static const char col_gray2[]  = "#444444";
static const char col_gray3[]  = "#bbbbbb";
static const char col_gray4[]  = "#eeeeee";
static const char col_pink[]   = "#ff79c6";
static const char col_cyan[]   = "#8be9fd";
static const char *colors[][3] = {
    /*                   fg          bg          border   */
    [SchemeNorm]     = { col_gray3,  col_gray1,  col_gray2 },
    [SchemeSel]      = { col_gray1,  col_pink,   col_pink  },
    [SchemeStatus]   = { col_cyan,   col_gray1,  "#000000" }, // Statusbar right {text,background,not used but cannot be empty}
    [SchemeTagsSel]  = { col_gray1,  col_cyan,   "#000000" }, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm] = { "#FFFFFF",  col_gray1,  "#000000" }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]  = { col_gray1,  col_pink,   "#000000" }, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm] = { "#FFFFFF",  col_gray1,  "#000000" }, // infobar middle  unselected {text,background,not used but cannot be empty}
                                                              // colors[SchemeInfoSel][2] actually uses colors[SchemeSel][2]???
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", };

static const Rule rules[] = {
    /* xprop(1):
     *    WM_CLASS(STRING) = instance, class
     *    WM_NAME(STRING) = title
     */
    /* class            instance    title       tags mask   isfloating  monitor */
    { "firefox",        NULL,       NULL,       1 << 1,     0,          0 },
    { "google-chrome",  NULL,       NULL,       1 << 1,     0,          0 },
    { "qutebrowser",    NULL,       NULL,       1 << 1,     0,          0 },
    { "discord",        NULL,       NULL,       1 << 2,     0,          0 },
    { "Steam",          NULL,       NULL,       1 << 3,     0,          0 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "fibonacci.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[-]",      tile },
    { "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
#define BGFOLDER "/media/shared/Pictures/Mathspapers"
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-p", "Run:", "-l", "10", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_pink, "-sf", "#000000", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *browsercmd[]  = { "firefox", NULL };
static const char *discordcmd[] = { "discord", NULL };
static const char *kakstupcmd[] = { "kak", "-d", "-s", "dwm", "-e", "cd", "/home/gethin", NULL };
static const char *kakcmd[] = { "st", "-e", "kak", "-c", "dwm", NULL };
static const char *xsetcmd[] = { "xset", "r", "rate", "300", "50", NULL };
static const char *statuscmd[] = { "zstatus", NULL };
static const char *fehbgcmd[] = { "/bin/sh", "-c", "while true; do feh --bg-scale --randomize "BGFOLDER"; sleep 30; done", NULL };

/* startup programs */
static const char** startup_programs[] = { browsercmd, discordcmd, kakstupcmd, xsetcmd, statuscmd, fehbgcmd, NULL };

static Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
    { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,                       XK_b,      spawn,          {.v = browsercmd } },
    { MODKEY,                       XK_e,      spawn,          {.v = kakcmd } },
    { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
    { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
    { MODKEY,                       XK_Tab,    view,           {0} },
    { MODKEY,                       XK_q,      killclient,     {0} },
    { MODKEY,                       XK_space,  setlayout,      {0} },
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY,                       XK_f,      togglefullscr,  {0} },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { MODKEY|ControlMask,           XK_J,      setgaps,        {.i = -10 } },
    { MODKEY|ControlMask,           XK_K,      setgaps,        {.i = +10 } },
    /* { MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } }, */
    { MODKEY,                       XK_0,      view,           {.ui = ~0} },
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    { MODKEY|ShiftMask,             XK_q,      quit,           {1} },
    { MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    /* ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} }, */
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

