# AchieveOS

This is NOT a linux distro, the contents are all made from zero (except for the asmlib)

Must install onto hard disk

### GPT partition UUIDs

I've defined some custom uuids, here is the list of UUIDs that I used:

| Name                | UUID                                 |
|---------------------|--------------------------------------|
| BIOS boot partition | 49464555-7349-7553-7065-724861726421 |
| APFS partition      | 7C3457EF-0000-11AA-AA11-00306543ECAC |
| SWAP partition      | 54776F48-536F-4157-504D-656D6F72793F |

### TODO list

Still got a lot to do!

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
- [ ] Gylphs
- [ ] APFS
- [x] Elf loader
- [ ] Dynamic library
- [ ] Streams
- [ ] Ncurses
- [ ] Port Nethack <-- Version 0.0.1 OS Complete
- [ ] Truetype
- [ ] USB keyboard

##### Misc

- [ ] Rust - Got half a std library implemented - very nice :)