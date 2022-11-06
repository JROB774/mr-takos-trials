@echo off
setlocal

if "%~1"=="win32" goto build_win32
if "%~1"=="web" goto build_web

echo please specify a platform to build for (win32 or web)...
goto end

:build_win32
echo ----------------------------------------

set defs=-D BUILD_NATIVE
set idir=-I ../../depends/sdl/include -I ../../depends/sdl_mixer/include -I ../../depends/nksdk/nklibs -I ../../depends/glew/include -I ../../depends/glew/source
set ldir=-libpath:../../depends/sdl/lib -libpath:../../depends/sdl_mixer/lib
set libs=SDL2main.lib SDL2.lib SDL2_mixer.lib opengl32.lib shell32.lib
set cflg=
set lflg=-incremental:no

if "%~2"=="release" (

    set cflg=%cflg% -O2
    set lflg=%lflg% -release -subsystem:windows
) else (
    set defs=%defs% -D BUILD_DEBUG -D SDL_MAIN_HANDLED
)

set defs=%defs% -D BUILD_NATIVE

if not exist binary\win32 mkdir binary\win32

copy depends\sdl_mixer\bin\*.dll binary\win32\ > NUL
copy depends\sdl\bin\*.dll binary\win32\ > NUL

pushd binary\win32
cl ../../source/platform.c %cflg% %defs% %idir% -Fe:tako.exe -link %lflg% %ldir% %libs%
popd

goto end

:build_web
echo ----------------------------------------

call depends\emsdk\emsdk install latest
call depends\emsdk\emsdk activate latest
call depends\emsdk\emsdk_env.bat

set defs=-D BUILD_WEB
set idir=-I ../../depends/nksdk/nklibs
set libs=-s WASM=1 -s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_OGG=1 -s USE_VORBIS=1 -lidbfs.js
set cflg=
set lflg=-s EXPORTED_FUNCTIONS="['_main', '_main_callback']" -s EXPORTED_RUNTIME_METHODS="['ccall']"

if not exist binary\web mkdir binary\web

pushd binary\web
emcc %libs% %idir% %cflg% %lflg% %defs% ../../source/platform.c -o tako.html
popd

goto end

:end

endlocal
