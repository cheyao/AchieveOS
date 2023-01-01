# AchieveOS

This is NOT a linux distro, the contents are all made from zero (except for the asmlib)

**Changing to RISC-V! Behold big changes!**

See Achieve Core at https://github.com/cheyao/Achieve-Core

## Why AchieveOS?

AchieveOS guarantees __no__ backward capabilities, it only implies with C17 and newer.
Doing this benefits us that we always get the best stuff, and not still be trapped in 16 bit
mode at boot even if no-one uses it, and it is 50 years old.

### GPT partition UUIDs

Here is the list of UUIDs that I used:

| Name                | UUID                                 |
|---------------------|--------------------------------------|
| BIOS boot partition | BC13C2FF-59E6-4262-A352-B275FD6F7172 |
| EXT2 partition      | 0FC63DAF-8483-4772-8E79-3D69D8477DE4 |
| SWAP partition      | 0657FD6D-A4AB-43C4-84E5-0933C84B4F4F |

### TODO list

Still got a lot to do!

- [ ] RISC-V OS with Core
- [ ] VGA lines and stuff <-- Version 0.0.1

Old:

- [x] VESA video mode
- [x] Paging
- [x] Switch to 64 bit mode
- [x] ATA PIO Read
- [x] ATAPI Read
- [x] ATAPI Write
- [x] 3 stage bootloader
- [x] Install from cdrom
- [x] Curves & lines
- [x] GPT
- [x] Glyphs
- [ ] HFS+
- [ ] Streams
- [ ] Port Nethack <-- Version 0.0.1 OS Complete
- [ ] Malloc
- [x] Elf loader
- [ ] Dynamic library
- [ ] Ncurses
- [ ] Truetype
- [ ] USB keyboard

##### Misc

- [ ] Rust - Got half a std library implemented - very nice :)

Vacation time!!
Nope, this project is not abandoned, i'm just taking a few days of vacation :)