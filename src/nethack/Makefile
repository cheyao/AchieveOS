#	NetHack Makefile.
# NetHack 3.6  Makefile.src    $NHDT-Date: 1550876124 2019/02/22 22:55:24 $  $NHDT-Branch: NetHack-3.6.2-beta01 $:$NHDT-Revision: 1.70 $
# Copyright (c) 2018 by Pasi Kallinen
# NetHack may be freely redistributed.  See license for details.

# Root of source tree:
NHSROOT=..

# newer makes predefine $(MAKE) to 'make' and do smarter processing of
# recursive make calls if $(MAKE) is used
# these makes allow $(MAKE) to be overridden by the environment if someone
# wants to (or has to) use something other than the standard make, so we do
# not want to unconditionally set $(MAKE) here
#
# unfortunately, some older makes do not predefine $(MAKE); if you have one of
# these, uncomment the following line
# (you will know that you have one if you get complaints about being unable
# to find 'makedefs')
# MAKE = make

# This makefile replaces the previous Makefile.unix, Makefile.xenix,
# Makefile.3B2, Makefile.att, and Makefile.tos.
# Set SYSTEM to one of:
#	'Sysunix'	-- generic UNIX]
SYSTEM = Sysunix

#
# Make sure that your bourne shell is specified here, as you have to spawn
# some of the commands (eg. depend) in bourne shell for them to work.
#
# For Systos users compiling on the ST, you'll either need a bourne shell
# clone or you'll need to do make depend, etc. by hand. In either case,
# the line below probably needs changing
SHELL=/bin/sh

# Pick the SYSSRC and SYSOBJ lines corresponding to your desired operating
# system.
#
# for UNIX systems
SYSSRC = sys/share/ioctl.c sys/share/unixtty.c sys/unix/unixmain.c \
	sys/unix/unixunix.c sys/unix/unixres.c
SYSOBJ = ioctl.o unixmain.o unixtty.o unixunix.o unixres.o

CC = x86_64-elf-gcc -O0 -fverbose-asm -nostdlib -nostdinc -fno-stack-protector -nostartfiles \
                    -nodefaultlibs -fno-builtin -fms-extensions -ffreestanding -g -mcmodel=large \
                    -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -I include/ -I ../

GNOMEINC=-I/usr/lib/glib/include -I/usr/lib/gnome-libs/include -Iwin/gnome

# we specify C preprocessor flags via CFLAGS; files built with default rules
# might include $(CPPFLAGS) which could get a value from user's environment;
# we avoid that by forcing it empty rather than by overriding default rules
CPPFLAGS =

# file for regular expression matching
REGEXOBJ = posixregex.o

# Set the WINSRC, WINOBJ, and WINLIB lines to correspond to your desired
# combination of windowing systems.  Also set windowing systems in config.h.
# Note that if you are including multiple tiled window systems, you don't
# want two copies of tile.o, so comment out all but the first.
#
# files for a straight tty port using no native windowing system
WINTTYSRC = win/tty/getline.c win/tty/termcap.c win/tty/topl.c \
	win/tty/wintty.c
WINTTYOBJ = getline.o termcap.o topl.o wintty.o

# if you defined RANDOM in unixconf.h/tosconf.h since your system did not come
# with a reasonable random number generator
RANDOBJ = random.o

# when using 'makedefs -v', also force dat/gitinfo.txt to be up to date;
# changing this to 0 will change the behavior to only make that file if
# it doesn't already exist; to skip it completely, create an empty file
# of that name and also set this to 0; there shouldn't be any need to
# skip it--if nethack's sources don't reside in a git repository than
# the script which creates that file will fail benignly and 'makedefs -v'
# will proceed without it
GITINFO=1

# if you change this to 1, feedback while building will omit -Dthis -Wthat
# -Isomewhere so that each file being compiled is listed on one short line;
# it requires support for '$<' in rules with more than one prerequisite
# (rather than just in suffix default rule), such as is implemented by
# gnu make and others which have picked up its extensions;
# allowed values are 0, 1, and empty (which behaves like 0)
QUIETCC=0

# ----------------------------------------
#
# Nothing below this line should have to be changed.
#
# Other things that have to be reconfigured are in config.h,
# {unixconf.h, pcconf.h, tosconf.h}, and possibly system.h

# Verbosity definitions, begin
#   Set QUIETCC=1 above to output less feedback while building.
#   CC and CXX obey verbosity, LD and LINK don't.
#   AT is @ when not verbose, empty otherwise
ACTUAL_CC := $(CC)
ACTUAL_CXX := $(CXX)
ACTUAL_LD := $(LD)
ACTUAL_LINK := $(LINK)

CC_V0 = $(ACTUAL_CC)
CC_V = $(CC_V0)
CC_V1 = @echo "[CC] $<"; $(ACTUAL_CC)
CC = $(CC_V$(QUIETCC))

CXX_V0 = $(ACTUAL_CXX)
CXX_V = $(CXX_V0)
CXX_V1 = @echo "[CXX] $<"; $(ACTUAL_CXX)
CXX = $(CXX_V$(QUIETCC))

# LD and LINK might be based on invoking CC and may not be able to substitute
# for QUIETCC, so feedback from them is handled differently (via $AT)
LD = x86_64-elf-ld
LINK = $(ACTUAL_LINK)

AT_V0 :=
AT_V := $(AT_V0)
AT_V1 := @
AT = $(AT_V$(QUIETCC))
# Verbosity, end

MAKEDEFS = util/makedefs

# timestamp files to reduce `make' overhead and shorten .o dependency lists
CONFIG_H = src/config.h-t
HACK_H	= src/hack.h-t

# all .c that are part of the main NetHack program and are not operating- or
# windowing-system specific
HACKCSRC = src/allmain.c src/alloc.c src/apply.c src/artifact.c src/attrib.c src/ball.c src/bones.c \
	   src/botl.c src/cmd.c src/dbridge.c src/decl.c src/detect.c src/dig.c src/display.c src/dlb.c src/do.c \
	   src/do_name.c src/do_wear.c src/dog.c src/dogmove.c src/dokick.c src/dothrow.c src/drawing.c \
	   src/dungeon.c eat.c end.c engrave.c exper.c explode.c extralev.c \
	   src/files.c fountain.c hack.c hacklib.c invent.c isaac64.c light.c \
	   src/lock.c mail.c makemon.c mapglyph.c mcastu.c mhitm.c mhitu.c \
	   src/minion.c mklev.c mkmap.c \
	   src/mkmaze.c mkobj.c mkroom.c mon.c mondata.c monmove.c monst.c \
	   src/mplayer.c mthrowu.c muse.c music.c o_init.c objects.c objnam.c \
	   src/options.c pager.c pickup.c pline.c polyself.c potion.c pray.c \
	   src/priest.c quest.c questpgr.c read.c rect.c region.c restore.c \
	   src/rip.c rnd.c role.c rumors.c save.c shk.c shknam.c sit.c sounds.c \
	   src/sp_lev.c spell.c steal.c steed.c sys.c teleport.c timeout.c \
	   src/topten.c track.c trap.c u_init.c \
	   src/uhitm.c vault.c version.c vision.c weapon.c were.c wield.c \
	   src/windows.c wizard.c worm.c worn.c write.c zap.c

# all operating-system-dependent .c (for dependencies and such)
SYSCSRC = sys/atari/tos.c sys/share/pcmain.c sys/share/pcsys.c \
	sys/share/pctty.c sys/share/pcunix.c \
	sys/share/pmatchregex.c sys/share/posixregex.c \
	sys/share/random.c \
	sys/share/ioctl.c sys/share/unixtty.c sys/unix/unixmain.c \
	sys/unix/unixunix.c sys/unix/unixres.c sys/be/bemain.c
SYSCXXSRC = sys/share/cppregex.cpp

# generated source files (tile.c is handled separately via WINxxxSRC)
GENCSRC = vis_tab.c	#tile.c

# all windowing-system-dependent .c (for dependencies and such)
WINCSRC = $(WINTTYSRC) $(WINCURSESSRC) $(WINX11SRC) $(WINGNOMESRC) $(WINGEMSRC)
# all windowing-system-dependent .cpp (for dependencies and such)
WINCXXSRC = $(WINQTSRC) $(WINQT4SRC) $(WINBESRC)

# Files for window system chaining. Requires SYSCF; include via HINTSRC/HINTOBJ
CHAINSRC = win/chain/wc_chainin.c win/chain/wc_chainout.c \
	   win/chain/wc_trace.c
CHAINOBJ = wc_chainin.o wc_chainout.o wc_trace.o

# .c files for this version (for date.h)
VERSOURCES = $(HACKCSRC) $(SYSSRC) $(WINSRC) $(CHAINSRC) $(GENCSRC)

# .c files for all versions using this Makefile (for lint and tags)
CSOURCES = $(HACKCSRC) $(SYSCSRC) $(WINCSRC) $(CHAINSRC) $(GENCSRC)


# all .h files except date.h, onames.h, pm.h, and vis_tab.h which would
# cause dependency loops if run through "make depend"
# and dgn_comp.h, dgn_file.h, lev_comp.h, special level & dungeon files.
#
HACKINCL = align.h amiconf.h artifact.h artilist.h attrib.h beconf.h botl.h \
	color.h config.h config1.h context.h coord.h decl.h def_os2.h \
	display.h dlb.h dungeon.h engrave.h extern.h flag.h func_tab.h \
	global.h hack.h lev.h lint.h macconf.h mextra.h mfndpos.h micro.h \
	mkroom.h \
	monattk.h mondata.h monflag.h monst.h monsym.h obj.h objclass.h \
	os2conf.h patchlevel.h pcconf.h permonst.h prop.h rect.h region.h \
	rm.h sp_lev.h spell.h sys.h system.h tcap.h timeout.h tosconf.h \
	tradstdc.h trampoli.h trap.h unixconf.h vision.h vmsconf.h wintty.h \
	wincurs.h winX.h winprocs.h wintype.h you.h youprop.h

HSOURCES = $(HACKINCL) date.h onames.h pm.h vis_tab.h \
		lev_comp.h dgn_comp.h dgn_file.h

# the following .o's _must_ be made before any others (for makedefs)
FIRSTOBJ = src/monst.o src/objects.o

HOBJ = $(FIRSTOBJ) allmain.o alloc.o apply.o artifact.o attrib.o ball.o \
	bones.o botl.o cmd.o dbridge.o decl.o detect.o dig.o display.o dlb.o \
	do.o do_name.o do_wear.o dog.o dogmove.o dokick.o dothrow.o \
	drawing.o dungeon.o eat.o end.o engrave.o exper.o explode.o \
	extralev.o files.o fountain.o hack.o hacklib.o invent.o isaac64.o \
	light.o lock.o mail.o makemon.o mapglyph.o mcastu.o mhitm.o mhitu.o \
	minion.o mklev.o mkmap.o \
	mkmaze.o mkobj.o mkroom.o mon.o mondata.o monmove.o \
	mplayer.o mthrowu.o muse.o music.o o_init.o objnam.o options.o \
	pager.o pickup.o pline.o polyself.o potion.o pray.o priest.o \
	quest.o questpgr.o read.o rect.o region.o restore.o rip.o rnd.o \
	role.o rumors.o save.o shk.o shknam.o sit.o sounds.o sp_lev.o spell.o \
	sys.o \
	steal.o steed.o teleport.o timeout.o topten.o track.o trap.o u_init.o \
	uhitm.o vault.o vision.o vis_tab.o weapon.o were.o wield.o windows.o \
	wizard.o worm.o worn.o write.o zap.o \
	$(REGEXOBJ) $(RANDOBJ) $(SYSOBJ) $(WINOBJ) $(HINTOBJ) version.o
# the .o files from the HACKCSRC, SYSSRC, and WINSRC lists

nethack:	$(SYSTEM)
	@echo "nethack is up to date."

Sysunix: $(HOBJ) Makefile
	@echo "Linking nethack."
	$(AT)$(LINK) $(LFLAGS) -o nethack $(HOBJ) $(WINLIB) $(LIBS)
	@touch Sysunix

DUMB.Setup:	include/extern.h
	cp include/extern.h include/extern.h.BAK
	cat include/extern.h | \
		sed -e '/^E\ int\ /!b' \
			-e '/[^;/ 	]$$/N' \
			-e '/[(][*]occupation[)]/b' \
			-e '/[(][*]afternmv[)]/b' \
			-e '/float_down/b' \
			-e '/done1/b' \
			-e '/identify/b' \
			-e '/Hear_again/b' \
			-e '/hangup/b' \
			-e 's/^\(.*\)$$/\/\* \1 \/\*\*\//' | \
		sed -e '/^E\ void\ /!b' \
			-e '/[^;/ 	]$$/N' \
			-e 's/^\(.*\)$$/\/\* \1 \/\*\*\//' \
				>include/extern.DUMB
	cp include/extern.DUMB include/extern.h
	@touch DUMB.Setup

all: nethack


#	dependencies for makedefs and its outputs, which the util
#	Makefile is responsible for keeping up to date
#

# special rules, to force update of makedefs, real dependencies should be
# below in the 'make depend' output.
monst.o:
	$(CC) $(CFLAGS) -c monst.c
	@rm -f $(MAKEDEFS)

objects.o:
	$(CC) $(CFLAGS) -c objects.c
	@rm -f $(MAKEDEFS)

#	build monst.o and objects.o before executing '$(MAKE) makedefs'
$(MAKEDEFS): $(FIRSTOBJ) \
		util/makedefs.c $(CONFIG_H) include/permonst.h \
		include/objclass.h include/monsym.h \
		include/artilist.h include/dungeon.h include/obj.h \
		include/monst.h include/you.h include/flag.h \
		include/dlb.h include/patchlevel.h include/qtext.h
	$(CC) $(CFLAGS) util/makedefs.c -o util/makedefs.o

#	Source files created by 'makedefs' at build time.
#	Each is given an artificial dependency upon the one before
#	so that parallel makes will have to build them sequentially.
#	(More for documentation than effect; 'make' should know not
#	to try to build $(MAKEDEFS) for bar.h while it is in the
#	process of building it for foo.h.)
include/onames.h: $(MAKEDEFS)
	@( cd util ; $(MAKE) include/onames.h )
include/pm.h: $(MAKEDEFS) include/onames.h
	@( cd util ; $(MAKE) include/pm.h )
include/vis_tab.h: $(MAKEDEFS) include/pm.h
	@( cd util ; $(MAKE) include/vis_tab.h )
# makedefs -z makes both vis_tab.h and vis_tab.c, but writes the .h first
vis_tab.c: include/vis_tab.h
#	Created at build time for configurations which support tiles,
#	but not by makedefs so not connected to the others.
tile.c: win/share/tilemap.c $(HACK_H)
	@( cd util ; $(MAKE) src/tile.c )

win/gnome/gn_rip.h: win/X11/rip.xpm
	cp win/X11/rip.xpm win/gnome/gn_rip.h

#	date.h should be remade any time any of the source or include code
#	is modified.  Unfortunately, this would make the contents of this
#	file far more complex.  Since "hack.h" depends on most of the include
#	files, we kludge around this by making date.h dependent on hack.h,
#	even though it doesn't include this file.
#	Do NOT include dat/gitinfo.txt as either a prerequisite or target.
#	'makedefs -v' processes it when present and ignores it if not.
#
#	hack.h depends on makedefs' output, so we know makedefs will be
#	up to date before being executed
include/date.h:	$(VERSOURCES) $(HACK_H)
	-$(SHELL) sys/unix/gitinfo.sh $(GITINFO) #before 'makedefs -v'
	util/makedefs -v


lint:
# lint cannot have -p here because (i) capitals are meaningful:
# [Ww]izard, (ii) identifiers may coincide in the first six places:
# doweararm() versus dowearring().
# _flsbuf comes from <stdio.h>, a bug in the system libraries.
	@echo lint -axbh -DLINT ...
	@lint -axbh -Iinclude -DLINT $(CSOURCES) | sed '/_flsbuf/d'


tags: $(CSOURCES)
	@echo ctags -tw ...
	@ctags -tw $(CSOURCES)
	@( cd include ; ctags -tw $(HSOURCES) )
	@( cd util ; $(MAKE) tags )

clean:
	-rm -f *.o $(HACK_H) $(CONFIG_H)

spotless: clean
	-rm -f a.out core nethack Sys*
	-rm -f include/date.h include/onames.h include/pm.h
	-rm -f include/vis_tab.h vis_tab.c tile.c *.moc
	-rm -f win/gnome/gn_rip.h


depend: sys/unix/depend.awk \
		$(SYSCSRC) $(WINCSRC) $(SYSCXXSRC) $(WINCXXSRC) \
		$(CHAINSRC) $(GENCSRC) $(HACKCSRC)
	$(AWK) -f sys/unix/depend.awk include/*.h \
		$(SYSCSRC) $(WINCSRC) $(SYSCXXSRC) $(WINCXXSRC) \
		$(CHAINSRC) $(GENCSRC) $(HACKCSRC) >makedep
	@echo '/^# DO NOT DELETE THIS LINE OR CHANGE ANYTHING BEYOND IT/+2,$$d' >eddep
	@echo '$$r makedep' >>eddep
	@echo 'w' >>eddep
	@cp Makefile Makefile.bak
	ed - Makefile < eddep
	@rm -f eddep makedep
	@echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	@echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	@echo '# see make depend above' >> Makefile
	- diff Makefile.bak Makefile
	@rm -f Makefile.bak

# DO NOT DELETE THIS LINE OR CHANGE ANYTHING BEYOND IT

# config.h timestamp
$(CONFIG_H): include/config.h include/config1.h include/tradstdc.h \
		include/global.h include/coord.h include/vmsconf.h \
		include/system.h include/unixconf.h include/os2conf.h \
		include/micro.h include/pcconf.h include/tosconf.h \
		include/amiconf.h include/macconf.h include/beconf.h \
		include/wceconf.h include/ntconf.h
	touch $(CONFIG_H)
# hack.h timestamp
$(HACK_H): include/hack.h $(CONFIG_H) include/lint.h include/align.h \
		include/dungeon.h include/monsym.h include/mkroom.h \
		include/objclass.h include/youprop.h include/prop.h \
		include/permonst.h include/monattk.h \
		include/monflag.h include/mondata.h include/pm.h \
		include/wintype.h include/context.h include/decl.h \
		include/quest.h include/spell.h include/color.h \
		include/obj.h include/you.h include/attrib.h \
		include/monst.h include/mextra.h include/skills.h \
		include/onames.h include/timeout.h include/trap.h \
		include/flag.h include/rm.h include/vision.h \
		include/display.h include/engrave.h include/rect.h \
		include/region.h include/winprocs.h include/botl.h \
		include/sys.h include/wintty.h include/trampoli.h
	touch $(HACK_H)
#
tos.o: sys/atari/tos.c $(HACK_H) include/tcap.h
	$(CC) $(CFLAGS) -c -o $@ sys/atari/tos.c
pcmain.o: sys/share/pcmain.c $(HACK_H) include/dlb.h
	$(CC) $(CFLAGS) -c -o $@ sys/share/pcmain.c
pcsys.o: sys/share/pcsys.c $(HACK_H)
	$(CC) $(CFLAGS) -c -o $@ sys/share/pcsys.c
pctty.o: sys/share/pctty.c $(HACK_H)
	$(CC) $(CFLAGS) -c -o $@ sys/share/pctty.c
pcunix.o: sys/share/pcunix.c $(HACK_H)
	$(CC) $(CFLAGS) -c -o $@ sys/share/pcunix.c
pmatchregex.o: sys/share/pmatchregex.c $(HACK_H)
	$(CC) $(CFLAGS) -c -o $@ sys/share/pmatchregex.c
posixregex.o: sys/share/posixregex.c $(HACK_H)
	$(CC) $(CFLAGS) -c -o $@ sys/share/posixregex.c
random.o: sys/share/random.c $(HACK_H)
	$(CC) $(CFLAGS) -c -o $@ sys/share/random.c
ioctl.o: sys/share/ioctl.c $(HACK_H) include/tcap.h
	$(CC) $(CFLAGS) -c -o $@ sys/share/ioctl.c
unixtty.o: sys/share/unixtty.c $(HACK_H)
	$(CC) $(CFLAGS) -c -o $@ sys/share/unixtty.c
unixmain.o: sys/unix/unixmain.c $(HACK_H) include/dlb.h
	$(CC) $(CFLAGS) -c -o $@ sys/unix/unixmain.c
unixunix.o: sys/unix/unixunix.c $(HACK_H)
	$(CC) $(CFLAGS) -c -o $@ sys/unix/unixunix.c
unixres.o: sys/unix/unixres.c $(CONFIG_H)
	$(CC) $(CFLAGS) -c -o $@ sys/unix/unixres.c
bemain.o: sys/be/bemain.c $(HACK_H) include/dlb.h
	$(CC) $(CFLAGS) -c -o $@ sys/be/bemain.c
getline.o: win/tty/getline.c $(HACK_H) include/func_tab.h
	$(CC) $(CFLAGS) -c -o $@ win/tty/getline.c
termcap.o: win/tty/termcap.c $(HACK_H) include/tcap.h
	$(CC) $(CFLAGS) -c -o $@ win/tty/termcap.c
topl.o: win/tty/topl.c $(HACK_H) include/tcap.h
	$(CC) $(CFLAGS) -c -o $@ win/tty/topl.c
wintty.o: win/tty/wintty.c $(HACK_H) include/dlb.h include/tcap.h
	$(CC) $(CFLAGS) -c -o $@ win/tty/wintty.c
cursmain.o: win/curses/cursmain.c $(HACK_H) include/patchlevel.h \
		include/wincurs.h
	$(CC) $(CFLAGS) -c -o $@ win/curses/cursmain.c
curswins.o: win/curses/curswins.c $(HACK_H) include/wincurs.h \
		win/curses/curswins.h
	$(CC) $(CFLAGS) -c -o $@ win/curses/curswins.c
cursmisc.o: win/curses/cursmisc.c $(HACK_H) include/wincurs.h \
		win/curses/cursmisc.h include/func_tab.h include/dlb.h
	$(CC) $(CFLAGS) -c -o $@ win/curses/cursmisc.c
cursdial.o: win/curses/cursdial.c $(HACK_H) include/wincurs.h \
		win/curses/cursdial.h include/func_tab.h
	$(CC) $(CFLAGS) -c -o $@ win/curses/cursdial.c
cursstat.o: win/curses/cursstat.c $(HACK_H) include/wincurs.h \
		win/curses/cursstat.h
	$(CC) $(CFLAGS) -c -o $@ win/curses/cursstat.c
cursinit.o: win/curses/cursinit.c $(HACK_H) include/wincurs.h \
		win/curses/cursinit.h
	$(CC) $(CFLAGS) -c -o $@ win/curses/cursinit.c
cursmesg.o: win/curses/cursmesg.c $(HACK_H) include/wincurs.h \
		win/curses/cursmesg.h
	$(CC) $(CFLAGS) -c -o $@ win/curses/cursmesg.c
cursinvt.o: win/curses/cursinvt.c $(HACK_H) include/wincurs.h \
		win/curses/cursinvt.h
	$(CC) $(CFLAGS) -c -o $@ win/curses/cursinvt.c
Window.o: win/X11/Window.c include/xwindowp.h include/xwindow.h \
		$(CONFIG_H) include/lint.h
	$(CC) $(CFLAGS) $(X11CFLAGS) -c -o $@ win/X11/Window.c
dialogs.o: win/X11/dialogs.c $(CONFIG_H) include/lint.h
	$(CC) $(CFLAGS) $(X11CFLAGS) -c -o $@ win/X11/dialogs.c
winX.o: win/X11/winX.c $(HACK_H) include/winX.h include/dlb.h \
		include/xwindow.h win/X11/nh72icon win/X11/nh56icon \
		win/X11/nh32icon
	$(CC) $(CFLAGS) $(X11CFLAGS) -c -o $@ win/X11/winX.c
winmap.o: win/X11/winmap.c include/xwindow.h $(HACK_H) include/dlb.h \
		include/winX.h include/tile2x11.h
	$(CC) $(CFLAGS) $(X11CFLAGS) -c -o $@ win/X11/winmap.c
winmenu.o: win/X11/winmenu.c $(HACK_H) include/winX.h
	$(CC) $(CFLAGS) $(X11CFLAGS) -c -o $@ win/X11/winmenu.c
winmesg.o: win/X11/winmesg.c include/xwindow.h $(HACK_H) include/winX.h
	$(CC) $(CFLAGS) $(X11CFLAGS) -c -o $@ win/X11/winmesg.c
winmisc.o: win/X11/winmisc.c $(HACK_H) include/func_tab.h \
		include/winX.h
	$(CC) $(CFLAGS) $(X11CFLAGS) -c -o $@ win/X11/winmisc.c
winstat.o: win/X11/winstat.c $(HACK_H) include/winX.h include/xwindow.h
	$(CC) $(CFLAGS) $(X11CFLAGS) -c -o $@ win/X11/winstat.c
wintext.o: win/X11/wintext.c $(HACK_H) include/winX.h include/xwindow.h
	$(CC) $(CFLAGS) $(X11CFLAGS) -c -o $@ win/X11/wintext.c
winval.o: win/X11/winval.c $(HACK_H) include/winX.h
	$(CC) $(CFLAGS) $(X11CFLAGS) -c -o $@ win/X11/winval.c
tile.o: tile.c $(HACK_H)
gnaskstr.o: win/gnome/gnaskstr.c win/gnome/gnaskstr.h \
		win/gnome/gnmain.h
	$(CC) $(CFLAGS) $(GNOMEINC) -c -o $@ win/gnome/gnaskstr.c
gnbind.o: win/gnome/gnbind.c win/gnome/gnbind.h win/gnome/gnmain.h \
		win/gnome/gnmenu.h win/gnome/gnaskstr.h \
		win/gnome/gnyesno.h
	$(CC) $(CFLAGS) $(GNOMEINC) -c -o $@ win/gnome/gnbind.c
gnglyph.o: win/gnome/gnglyph.c win/gnome/gnglyph.h include/tile2x11.h
	$(CC) $(CFLAGS) $(GNOMEINC) -c -o $@ win/gnome/gnglyph.c
gnmain.o: win/gnome/gnmain.c win/gnome/gnmain.h win/gnome/gnsignal.h \
		win/gnome/gnbind.h win/gnome/gnopts.h $(HACK_H) \
		include/date.h
	$(CC) $(CFLAGS) $(GNOMEINC) -c -o $@ win/gnome/gnmain.c
gnmap.o: win/gnome/gnmap.c win/gnome/gnmap.h win/gnome/gnglyph.h \
		win/gnome/gnsignal.h $(HACK_H)
	$(CC) $(CFLAGS) $(GNOMEINC) -c -o $@ win/gnome/gnmap.c
gnmenu.o: win/gnome/gnmenu.c win/gnome/gnmenu.h win/gnome/gnmain.h \
		win/gnome/gnbind.h include/func_tab.h
	$(CC) $(CFLAGS) $(GNOMEINC) -c -o $@ win/gnome/gnmenu.c
gnmesg.o: win/gnome/gnmesg.c win/gnome/gnmesg.h win/gnome/gnsignal.h
	$(CC) $(CFLAGS) $(GNOMEINC) -c -o $@ win/gnome/gnmesg.c
gnopts.o: win/gnome/gnopts.c win/gnome/gnopts.h win/gnome/gnglyph.h \
		win/gnome/gnmain.h win/gnome/gnmap.h $(HACK_H)
	$(CC) $(CFLAGS) $(GNOMEINC) -c -o $@ win/gnome/gnopts.c
gnplayer.o: win/gnome/gnplayer.c win/gnome/gnplayer.h \
		win/gnome/gnmain.h $(HACK_H)
	$(CC) $(CFLAGS) $(GNOMEINC) -c -o $@ win/gnome/gnplayer.c
gnsignal.o: win/gnome/gnsignal.c win/gnome/gnsignal.h \
		win/gnome/gnmain.h
	$(CC) $(CFLAGS) $(GNOMEINC) -c -o $@ win/gnome/gnsignal.c
gnstatus.o: win/gnome/gnstatus.c win/gnome/gnstatus.h \
		win/gnome/gnsignal.h win/gnome/gn_xpms.h \
		win/gnome/gnomeprv.h
	$(CC) $(CFLAGS) $(GNOMEINC) -c -o $@ win/gnome/gnstatus.c
gntext.o: win/gnome/gntext.c win/gnome/gntext.h win/gnome/gnmain.h \
		win/gnome/gn_rip.h
	$(CC) $(CFLAGS) $(GNOMEINC) -c -o $@ win/gnome/gntext.c
gnyesno.o: win/gnome/gnyesno.c win/gnome/gnbind.h win/gnome/gnyesno.h
	$(CC) $(CFLAGS) $(GNOMEINC) -c -o $@ win/gnome/gnyesno.c
gnworn.o: win/gnome/gnworn.c win/gnome/gnworn.h win/gnome/gnglyph.h \
		win/gnome/gnsignal.h win/gnome/gnomeprv.h
	$(CC) $(CFLAGS) $(GNOMEINC) -c -o $@ win/gnome/gnworn.c
wingem.o: win/gem/wingem.c $(HACK_H) include/func_tab.h include/dlb.h \
		include/patchlevel.h include/wingem.h
	$(CC) $(CFLAGS) -c -o $@ win/gem/wingem.c
wingem1.o: win/gem/wingem1.c include/gem_rsc.h include/load_img.h \
		include/gr_rect.h include/wintype.h include/wingem.h
	$(CC) $(CFLAGS) -c -o $@ win/gem/wingem1.c
load_img.o: win/gem/load_img.c include/load_img.h
	$(CC) $(CFLAGS) -c -o $@ win/gem/load_img.c
gr_rect.o: win/gem/gr_rect.c include/gr_rect.h
	$(CC) $(CFLAGS) -c -o $@ win/gem/gr_rect.c
tile.o: tile.c $(HACK_H)
cppregex.o: sys/share/cppregex.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ sys/share/cppregex.cpp
qt_win.o: win/Qt/qt_win.cpp $(HACK_H) include/func_tab.h \
		include/dlb.h include/patchlevel.h include/tile2x11.h \
		include/qt_win.h include/qt_clust.h include/qt_kde0.h \
		include/qt_xpms.h qt_win.moc qt_kde0.moc qttableview.moc
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt/qt_win.cpp
qt_clust.o: win/Qt/qt_clust.cpp include/qt_clust.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt/qt_clust.cpp
qttableview.o: win/Qt/qttableview.cpp include/qttableview.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt/qttableview.cpp
qt4bind.o: win/Qt4/qt4bind.cpp $(HACK_H) win/Qt4/qt4bind.h \
		win/Qt4/qt4click.h win/Qt4/qt4delay.h \
		win/Qt4/qt4xcmd.h win/Qt4/qt4key.h win/Qt4/qt4map.h \
		win/Qt4/qt4menu.h win/Qt4/qt4msg.h win/Qt4/qt4plsel.h \
		win/Qt4/qt4svsel.h win/Qt4/qt4set.h win/Qt4/qt4stat.h \
		win/Qt4/qt4streq.h win/Qt4/qt4yndlg.h \
		win/Qt4/qt4str.h include/dlb.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4bind.cpp
qt4click.o: win/Qt4/qt4click.cpp $(HACK_H) win/Qt4/qt4click.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4click.cpp
qt4clust.o: win/Qt4/qt4clust.cpp win/Qt4/qt4clust.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4clust.cpp
qt4delay.o: win/Qt4/qt4delay.cpp $(HACK_H) win/Qt4/qt4delay.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4delay.cpp
qt4glyph.o: win/Qt4/qt4glyph.cpp $(HACK_H) include/tile2x11.h \
		win/Qt4/qt4glyph.h win/Qt4/qt4set.h win/Qt4/qt4str.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4glyph.cpp
qt4icon.o: win/Qt4/qt4icon.cpp $(HACK_H) win/Qt4/qt4icon.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4icon.cpp
qt4inv.o: win/Qt4/qt4inv.cpp $(HACK_H) win/Qt4/qt4inv.h \
		win/Qt4/qt4glyph.h win/Qt4/qt4set.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4inv.cpp
qt4key.o: win/Qt4/qt4key.cpp $(HACK_H) win/Qt4/qt4key.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4key.cpp
qt4line.o: win/Qt4/qt4line.cpp $(HACK_H) win/Qt4/qt4line.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4line.cpp
qt4main.o: win/Qt4/qt4main.cpp $(HACK_H) include/patchlevel.h \
		win/Qt4/qt4main.h qt4main.moc win/Qt4/qt4bind.h \
		win/Qt4/qt4glyph.h win/Qt4/qt4inv.h win/Qt4/qt4key.h \
		win/Qt4/qt4map.h win/Qt4/qt4msg.h win/Qt4/qt4set.h \
		win/Qt4/qt4stat.h win/Qt4/qt4str.h qt4kde0.moc
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4main.cpp
qt4map.o: win/Qt4/qt4map.cpp $(HACK_H) win/Qt4/qt4map.h qt4map.moc \
		win/Qt4/qt4click.h win/Qt4/qt4glyph.h \
		include/qt_xpms.h win/Qt4/qt4set.h win/Qt4/qt4str.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4map.cpp
qt4menu.o: win/Qt4/qt4menu.cpp $(HACK_H) win/Qt4/qt4menu.h qt4menu.moc \
		win/Qt4/qt4glyph.h win/Qt4/qt4set.h \
		win/Qt4/qt4streq.h win/Qt4/qt4str.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4menu.cpp
qt4msg.o: win/Qt4/qt4msg.cpp $(HACK_H) win/Qt4/qt4msg.h qt4msg.moc \
		win/Qt4/qt4map.h win/Qt4/qt4set.h win/Qt4/qt4str.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4msg.cpp
qt4plsel.o: win/Qt4/qt4plsel.cpp $(HACK_H) win/Qt4/qt4plsel.h \
		qt4plsel.moc win/Qt4/qt4bind.h win/Qt4/qt4glyph.h \
		win/Qt4/qt4set.h win/Qt4/qt4str.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4plsel.cpp
qt4rip.o: win/Qt4/qt4rip.cpp $(HACK_H) win/Qt4/qt4rip.h \
		win/Qt4/qt4bind.h win/Qt4/qt4str.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4rip.cpp
qt4set.o: win/Qt4/qt4set.cpp $(HACK_H) win/Qt4/qt4set.h qt4set.moc \
		win/Qt4/qt4glyph.h win/Qt4/qt4str.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4set.cpp
qt4stat.o: win/Qt4/qt4stat.cpp $(HACK_H) win/Qt4/qt4stat.h qt4stat.moc \
		win/Qt4/qt4set.h win/Qt4/qt4str.h include/qt_xpms.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4stat.cpp
qt4str.o: win/Qt4/qt4str.cpp win/Qt4/qt4str.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4str.cpp
qt4streq.o: win/Qt4/qt4streq.cpp $(HACK_H) win/Qt4/qt4streq.h \
		win/Qt4/qt4str.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4streq.cpp
qt4svsel.o: win/Qt4/qt4svsel.cpp $(HACK_H) win/Qt4/qt4svsel.h \
		win/Qt4/qt4bind.h win/Qt4/qt4str.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4svsel.cpp
qt4win.o: win/Qt4/qt4win.cpp $(HACK_H) win/Qt4/qt4win.h \
		win/Qt4/qt4bind.h win/Qt4/qt4click.h \
		win/Qt4/qt4glyph.h win/Qt4/qt4inv.h win/Qt4/qt4key.h \
		win/Qt4/qt4icon.h win/Qt4/qt4map.h win/Qt4/qt4menu.h \
		win/Qt4/qt4msg.h win/Qt4/qt4set.h win/Qt4/qt4clust.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4win.cpp
qt4xcmd.o: win/Qt4/qt4xcmd.cpp $(HACK_H) include/func_tab.h \
		win/Qt4/qt4xcmd.h qt4xcmd.moc win/Qt4/qt4bind.h \
		win/Qt4/qt4set.h win/Qt4/qt4str.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4xcmd.cpp
qt4yndlg.o: win/Qt4/qt4yndlg.cpp $(HACK_H) win/Qt4/qt4yndlg.h \
		qt4yndlg.moc win/Qt4/qt4str.h
	$(CXX) $(CXXFLAGS) -c -o $@ win/Qt4/qt4yndlg.cpp
wc_chainin.o: win/chain/wc_chainin.c $(HACK_H)
	$(CC) $(CFLAGS) -c -o $@ win/chain/wc_chainin.c
wc_chainout.o: win/chain/wc_chainout.c $(HACK_H)
	$(CC) $(CFLAGS) -c -o $@ win/chain/wc_chainout.c
wc_trace.o: win/chain/wc_trace.c $(HACK_H) include/func_tab.h
	$(CC) $(CFLAGS) -c -o $@ win/chain/wc_trace.c
vis_tab.o: vis_tab.c $(CONFIG_H) include/vis_tab.h
allmain.o: src/allmain.c $(HACK_H)
alloc.o: src/alloc.c $(CONFIG_H)
apply.o: src/apply.c $(HACK_H)
artifact.o: src/artifact.c $(HACK_H) include/artifact.h include/artilist.h
attrib.o: src/attrib.c $(HACK_H)
ball.o: src/ball.c $(HACK_H)
bones.o: src/bones.c $(HACK_H) include/lev.h
botl.o: src/botl.c $(HACK_H)
cmd.o: src/cmd.c $(HACK_H) include/lev.h include/func_tab.h
dbridge.o: src/dbridge.c $(HACK_H)
decl.o: src/decl.c $(HACK_H)
detect.o: src/detect.c $(HACK_H) include/artifact.h
dig.o: src/dig.c $(HACK_H)
display.o: src/display.c $(HACK_H)
dlb.o: src/dlb.c $(CONFIG_H) include/dlb.h
do.o: src/do.c $(HACK_H) include/lev.h
do_name.o: src/do_name.c $(HACK_H)
do_wear.o: src/do_wear.c $(HACK_H)
dog.o: src/dog.c $(HACK_H)
dogmove.o: src/dogmove.c $(HACK_H) include/mfndpos.h
dokick.o: dokick.c $(HACK_H)
dothrow.o: dothrow.c $(HACK_H)
drawing.o: drawing.c $(HACK_H) include/tcap.h
dungeon.o: dungeon.c $(HACK_H) include/dgn_file.h include/dlb.h \
		include/lev.h
eat.o: eat.c $(HACK_H)
end.o: end.c $(HACK_H) include/lev.h include/dlb.h
engrave.o: engrave.c $(HACK_H) include/lev.h
exper.o: exper.c $(HACK_H)
explode.o: explode.c $(HACK_H)
extralev.o: extralev.c $(HACK_H)
files.o: files.c $(HACK_H) include/dlb.h #zlib.h
fountain.o: fountain.c $(HACK_H)
hack.o: hack.c $(HACK_H)
hacklib.o: hacklib.c $(HACK_H)
invent.o: invent.c $(HACK_H)
isaac64.o: isaac64.c $(CONFIG_H) include/isaac64.h include/integer.h
light.o: light.c $(HACK_H) include/lev.h
lock.o: lock.c $(HACK_H)
mail.o: mail.c $(HACK_H) include/mail.h
makemon.o: makemon.c $(HACK_H)
mapglyph.o: mapglyph.c $(HACK_H)
mcastu.o: mcastu.c $(HACK_H)
mhitm.o: mhitm.c $(HACK_H) include/artifact.h
mhitu.o: mhitu.c $(HACK_H) include/artifact.h
minion.o: minion.c $(HACK_H)
mklev.o: mklev.c $(HACK_H)
mkmap.o: mkmap.c $(HACK_H) include/sp_lev.h
mkmaze.o: mkmaze.c $(HACK_H) include/sp_lev.h include/lev.h
mkobj.o: mkobj.c $(HACK_H)
mkroom.o: mkroom.c $(HACK_H)
mon.o: mon.c $(HACK_H) include/mfndpos.h
mondata.o: mondata.c $(HACK_H)
monmove.o: monmove.c $(HACK_H) include/mfndpos.h include/artifact.h
monst.o: src/monst.c $(CONFIG_H) include/permonst.h include/align.h \
		include/monattk.h include/monflag.h include/monsym.h \
		include/color.h
mplayer.o: mplayer.c $(HACK_H)
mthrowu.o: mthrowu.c $(HACK_H)
muse.o: muse.c $(HACK_H)
music.o: music.c $(HACK_H) #interp.c
o_init.o: o_init.c $(HACK_H) include/lev.h
objects.o: objects.c $(CONFIG_H) include/obj.h include/objclass.h \
		include/prop.h include/skills.h include/color.h
objnam.o: objnam.c $(HACK_H)
options.o: options.c $(CONFIG_H) include/objclass.h include/flag.h \
		$(HACK_H) include/tcap.h
pager.o: pager.c $(HACK_H) include/dlb.h
pickup.o: pickup.c $(HACK_H)
pline.o: pline.c $(HACK_H)
polyself.o: polyself.c $(HACK_H)
potion.o: potion.c $(HACK_H)
pray.o: pray.c $(HACK_H)
priest.o: priest.c $(HACK_H) include/mfndpos.h
quest.o: quest.c $(HACK_H) include/qtext.h
questpgr.o: questpgr.c $(HACK_H) include/dlb.h include/qtext.h
read.o: read.c $(HACK_H)
rect.o: rect.c $(HACK_H)
region.o: region.c $(HACK_H) include/lev.h
restore.o: restore.c $(HACK_H) include/lev.h include/tcap.h
rip.o: rip.c $(HACK_H)
rnd.o: rnd.c $(HACK_H)
role.o: role.c $(HACK_H)
rumors.o: rumors.c $(HACK_H) include/lev.h include/dlb.h
save.o: save.c $(HACK_H) include/lev.h
shk.o: shk.c $(HACK_H)
shknam.o: shknam.c $(HACK_H)
sit.o: sit.c $(HACK_H) include/artifact.h
sounds.o: sounds.c $(HACK_H)
sp_lev.o: sp_lev.c $(HACK_H) include/dlb.h include/sp_lev.h
spell.o: spell.c $(HACK_H)
steal.o: steal.c $(HACK_H)
steed.o: steed.c $(HACK_H)
sys.o: sys.c $(HACK_H)
teleport.o: teleport.c $(HACK_H)
timeout.o: timeout.c $(HACK_H) include/lev.h
topten.o: topten.c $(HACK_H) include/dlb.h include/patchlevel.h
track.o: track.c $(HACK_H)
trap.o: trap.c $(HACK_H)
u_init.o: u_init.c $(HACK_H)
uhitm.o: uhitm.c $(HACK_H)
vault.o: vault.c $(HACK_H)
version.o: version.c $(HACK_H) include/dlb.h include/date.h \
		include/patchlevel.h
vision.o: vision.c $(HACK_H) include/vis_tab.h
weapon.o: weapon.c $(HACK_H)
were.o: were.c $(HACK_H)
wield.o: wield.c $(HACK_H)
windows.o: windows.c $(HACK_H) include/wingem.h include/winGnome.h
wizard.o: wizard.c $(HACK_H) include/qtext.h
worm.o: worm.c $(HACK_H) include/lev.h
worn.o: worn.c $(HACK_H)
write.o: write.c $(HACK_H)
zap.o: zap.c $(HACK_H)
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
