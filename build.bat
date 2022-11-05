@echo off
setlocal

if not exist binary\win32 mkdir binary\win32

copy depends\sdl_mixer\bin\win32\*.dll binary\win32\ > NUL
copy depends\sdl\bin\win32\*.dll binary\win32\ > NUL

set defs=-D BUILD_DEBUG -D SDL_MAIN_HANDLED
set idir=-I ../../depends/sdl/include -I ../../depends/sdl_mixer/include
set ldir=-libpath:../../depends/sdl/lib -libpath:../../depends/sdl_mixer/lib
set libs=SDL2main.lib SDL2.lib SDL2_mixer.lib
set cflg=
set lflg=-incremental:no

pushd binary\win32
cl ../../source/main.c %cflg% %defs% %idir% -Fe:tako.exe -link %lflg% %ldir% %libs%
popd

endlocal
