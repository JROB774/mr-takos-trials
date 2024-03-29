@echo off
setlocal

if "%~1"=="assets" goto build_assets
if "%~1"=="win32" goto build_win32
if "%~1"=="web" goto build_web
if "%~1"=="tools" goto build_tools

echo please specify build target (win32, web, tools, assets)...
goto end

:build_assets
echo ----------------------------------------

if "%~2"=="" goto asset_error

set platform=%~2

if not exist binary\%platform% mkdir binary\%platform%
python assets/build_assets.py %platform%

if %platform%==win32 tools\packer.exe

goto end

:asset_error
echo please specify build target (win32, web)...

goto end

:build_tools
echo ----------------------------------------

if not exist tools mkdir tools

pushd tools
cl ../source/tools/packer.c -I ../depends/nksdk -Fe:packer.exe
del *.obj
popd

goto end

:build_win32
echo ----------------------------------------

set defs=-D BUILD_NATIVE
set idir=-I ../../depends/sdl/include -I ../../depends/sdl_mixer/include -I ../../depends/nksdk -I ../../depends/glew/include -I ../../depends/glew/source -I ../../depends/stb
set ldir=-libpath:../../depends/sdl/lib -libpath:../../depends/sdl_mixer/lib
set libs=SDL2main.lib SDL2.lib SDL2_mixer.lib opengl32.lib shell32.lib
set cflg=
set lflg=-incremental:no

if "%~2"=="release" (
    set cflg=%cflg% -O2
    set lflg=%lflg% -release -subsystem:windows
) else (
    set defs=%defs% -D BUILD_DEBUG -D SDL_MAIN_HANDLED
    set cflg=%cflg% -Z7
)

if not exist binary\win32 mkdir binary\win32

copy depends\sdl_mixer\bin\*.dll binary\win32\ > NUL
copy depends\sdl\bin\*.dll binary\win32\ > NUL

pushd binary\win32
rc -i ../../redist/win32/res ../../redist/win32/res/icon.rc
cl ../../source/platform.c %cflg% %defs% %idir% -Fe:TAKO.exe -link %lflg% %ldir% %libs%  ../../redist/win32/res/icon.res
popd

goto end

:build_web
echo ----------------------------------------

call depends\emsdk\emsdk install latest
call depends\emsdk\emsdk activate latest
call depends\emsdk\emsdk_env.bat

set defs=-D BUILD_WEB
set idir=-I ../../depends/nksdk -I ../../depends/stb
set libs=-s WASM=1 -s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_OGG=1 -s USE_VORBIS=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -lidbfs.js
set cflg=
set lflg=--preload-file assets -s EXPORTED_FUNCTIONS="['_main', '_main_callback']" -s EXPORTED_RUNTIME_METHODS="['ccall']" -s ALLOW_MEMORY_GROWTH

if not exist binary\web mkdir binary\web

pushd binary\web
emcc %libs% %idir% %cflg% %lflg% %defs% ../../source/platform.c -o tako.html
popd

goto end

:end

endlocal
