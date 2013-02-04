===================
gfx2gba v0.8 README
===================

this tool converts 8 bit (256 color) or 4 bit (16 color) BMP or
8 bit (256 color) PCX graphic files to GBA useable data (raw,
tiles, map). the special thing about this tool is that it is able
to combine the palettes of several gfx files into one single
"master palette". it removes/remaps double colors. of course if
you try to combine 2 files with 256 unique colors each it will
fail, heh ... :p

Usage: gfx2gba [options] bmp/pcx files ...

Options are: -pPalettename
             -oOutputdir
             -fOutput format
             -TMetatilesize
             -tTilesize
             -cColordepth
             -CColor-Offset
             -vVRAM-Offset
             -bBank for map
             -m generate map (optimized)
             -M generate map (not optimized)
             -F turn of check for flipped tiles
             -rs output a rotate/scale BG screen map
             -P don't save palette data
             -x don't merge palettes
             -zt compress tile data *NEW*
             -zp compress palette data *NEW*
             -zm compress map data *NEW*
             -Z compress everything *NEW*

-p specifies the name of the master palette (default: master.pal)

-o specifies the output directory (default is current dir)

-f specifies the output format (raw/src/asm) (default is raw)

-T specifies the metatilesize (1/8/16/32/64) (default is 1, which
   means NO metatiling at all)

-t specifies the tilesize (1/8/16/32/64) (default is 1, which means
   NO tiling at all, giving you plain raw data for mode4/5 bg's)

-c specifies input/output color depth (16/256/32k) (default is 256)
   NOTE: in 256 color mode you can also READ 16 color images, but
         they'll be saved as 256 color images!
   NOTE2: 32k setting only works for OUTPUT of raw 15+1bit gfx data
          for use in MODE3/5, you can NOT READ other formats than
          16/256 color BMP or 256 color PCX. this tool was not
          designed for this mode anyway, so expect some bugs there
          (especially if you mix it with other options)

-C will add an offset (0-255) to the bitmap color indexes (default 0)
   it will NOT add the offset to color index #0 (transparent) !!!

-v specifies the offset to be added when saving tilemaps (default
   is 0)

-b specifies the color bank to use for 16 color tile maps (0-15)
   (default is 0)

NOTE: DO NOT USE SPACES between -p/-o/-t/-c/-v/-C/-b and it's parameter!!!
      use -t16 or something like that, but -t 16 won't work!!!

-m will generate an optimized tile map from the inputfile. optimized
   means it removes double tiles and also checks for horizontal,
   vertical and h+v flipped doubles. of course it sets the correct
   flags in the tile map data. the output .raw file will ONLY contain
   the used tiles (of course).

-M will generate a NON optimized tilemap

-F will turn off check for flipped tiles in map. only "real" doubles will
   be removed.

NOTE: activating map generation will change default tile size from 1 to
      a more GBA friendly size of 8. you can still set your own size using
      the -t option.
      fixed a stupid bug that produced an endless loop in v0.6 if your
      input gfx bitmap was >= 4mbyte.

-rs will force the map to be saved in 1 byte per tile format making it
    suitable for rotate/scale backgrounds. you lose color bank info and
    h/v flip info obviously and the max number of tiles is 256. but hey,
    you knew that already, don't ya?

-P will disable palette output at all

-x will prevent gfx2gba from merging/optimizing your palettes and save
   a separate .pal file for each input gfx file.

-zt will lz77 compress tile/gfx data

-zp will lz77 compress palette data

-zm will lz77 compress map data

-Z will lz77 compress everything

NOTE: all compression stuff is compatible to the GBA internal Lz77 depacker.
      if it fails to compress it will show you a warning and save the normal
      data.

==========================
Frequently Asked Questions
==========================

Q: so far so good, but what if i want to convert all my 36 .bmp files and
   12 .pcx files ?!
A: easy: gfx2gba *.pcx *.bmp
   could it be any simpler?! :)

Q: what -t value do i need for correct sprites and background tiles?
A: use -t8 to get what you need ...

Q: but i am using Linux for GBA dev?!
A: just use the included Linux binary (you may have to set exec flag)

Q: wtf is -v good for? why would i want to add an offset?!
A: HINT: to maximize the number of useable tiles.

Q: is it possible that your tool is the only free available one that
   also supports 16 color / 4 bit gfx?!
A: erm... well... seems so... looks like other tool coders ignore the
   presence of the 16 color modes on GBA ...

Q: v0.4 features are the same as in v0.3?! what's new or changed?!
A: *cough* this release has 2 major bugs fixed. bitmap export to 4bit
   (16 color) was broken (pixel swapped) (export to tiles was working,
   that's why i found this nasty bug so late) and my color optimizing
   stuff made ALL black (0x0000) colors transparent (thanks to Michael
   Jagger for yelling at me about this)

Q: soo, what's new in v0.5 ? i still can't see any new options?!
A: *cough*cough* the black color bug was nastier than i thought but at
   the end they all DIE ... or short: the last bug in color optimizing
   is removed now ... fixed bitmap remapping (last pixel wasn't remapped)
   and finally it compiles now without even a warning! :-)

Q: hey, i can mix all kind of options and gfx2gba doesn't complain about
   stuff that doesn't make sense?!
A: it is a tool to help you developing GBA stuff, not a brain replacement
   and it assumes you know at least a little bit what you are doing.

Q: I have a problem with 32k mode...
A: I don't care ... 32k mode is just a little add-on and not something
   high-priority for me ... :)

Q: hey, who's responsible for the color bank & rot/scale option? i don't
   think you're smart enough to think about 'em yourself!?
A: sad but true ... thanks to Alex D. for this obviously useful ideas.
   cheers mate, keep the ideas coming (even if they arrive in my inbox
   at 00:30 in the morning)!

Q: are there any other people who are giving you their valuable ideas
   free of charge?
A: yeah! many thanks to Dennis R. for the idea to change some of the
   default values and kicking my butt to implement C/C++ array output
   at least. and then we have Bengt Johannesson who found out that the
   symbol name creation was f*cked when you used files not placed in
   the current directory (it had the complete path in the label name
   instead of just the filename) ...

Q: who wanted the l33t flipped tile check to be removable!?
A: Nikita Mikros needed it so i added it ... :)

Q: metatiling? wow! cool name for a feature, BUT WHAT THE HELL IS IT?!
A: it more or less means that your input gfx will get tiled twice. for
   example my game "engine" is building large objects from 16x16 sprites.
   but 16x16 sprites have to made from 8x8 data internally anyway. so
   with a normal tool you have to group your objects in 16xY gfx to get
   a good result. now here comes metatiling into play: it first tiles
   your gfx into X*X and then in Y*Y. so -T16 -t8 will split up the gfx
   into 16x16 blocks first and then this blocks into 8x8 blocks ...
   i hope you get the idea ... :-)

Q: who's responsible for the compressing stuff?
A: thanks to Tony McB. for permisson to use his packer source!

=======
Contact
=======

any questions, reports, suggestions are welcome ...
contact me at: markus@console-dev.de

njoy another cool tool from your friends at MUPS!!!
