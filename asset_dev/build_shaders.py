import os

outdir_gl = "assets/shaders/gl_330/"
outdir_es = "assets/shaders/es_300/"

indir = "asset_dev/raw_shaders/"

if not os.path.isdir(outdir_gl):
    os.makedirs(outdir_gl)
if not os.path.isdir(outdir_es):
    os.makedirs(outdir_es)

files = os.listdir(path=indir)
for file in files:
    infile = os.path.join(indir, file)
    if os.path.isfile(infile):
        if infile.split(".")[1] == "shader":
            print("building shader {}".format(infile))
            outfile_gl = os.path.join(outdir_gl, file)
            outfile_es = os.path.join(outdir_es, file)
            with open(infile, "r") as inshader:
                src = inshader.read()
                with open(outfile_gl, "w") as outshader:
                    outshader.write(src)
                src = str.replace(src, "layout (location = 0) ", "")
                src = "precision mediump float;\n\n" + src
                with open(outfile_es, "w") as outshader:
                    outshader.write(src)
