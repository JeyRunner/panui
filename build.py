#!/usr/bin/python

# ############################
# build crossplatform via toolchain in cmake/

from optparse import OptionParser
import sys, getopt, os, glob, shutil
from subprocess import call

# VAR
ANDROID_ABIS = [  'armeabi', #'armeabi-v7a',
                  'armeabi-v7a with NEON', #'armeabi-v7a with VFPV3',
                  #'armeabi-v6 with VFP',
                  'x86',
                  #'arm64-v8a',
                  'x86_64',
                  #'mips64'
                  ]
ANDROID_TOOLCHAIN = 'cmake/android.toolchain.cmake'

error = False
packing = True
androidRun = False
androidDevice = ""
androidInstall = False
clean = True
rootBuild = ""
rootSrc = ""

def main(argv):
    global packing, androidInstall, androidRun, clean, rootBuild, rootSrc, ANDROID_TOOLCHAIN

    parser = OptionParser()
    parser.add_option("-a", "--android", dest="android",
                      help="cross compile for all ABI for android API('android-NUMBER')", metavar="API")
    parser.add_option("--androidABI", dest="androidAbi",
                      help="set static abi for android compile", metavar="ABI")
    parser.add_option("--androidInstall", dest="androidInstall", action="store_true",
                      help="install app", metavar="ABI")
    parser.add_option("--androidRun", dest="androidRun", action="store_true",
                      help="run app", metavar="ABI")


    parser.add_option("-w", "--windows", dest="windows", action="store_true",
                      help="cross compile for windows (coming soon)")
    parser.add_option("-n", "--native", dest="native", action="store_true",
                      help="build for host system")
    parser.add_option("-c", "--clean", dest="clean", action="store_true",
                      help="cleans only cmake files first, not compiled files")

    parser.add_option("--no-pack", dest="noPack", action="store_true",
                      help="not create package android application, only compile")
    (options, args) = parser.parse_args()


    packing = options.noPack is None
    clean = options.clean is not None
    androidInstall = options.androidInstall is not None
    androidRun = options.androidRun is not None
    if androidRun:
        androidDevice = options.androidRun

    rootSrc = os.getcwd()

    if clean:
        # remove old
        if os.path.exists('build/'):
            shutil.rmtree('build/', ignore_errors=True)

    # go into build dir
    if not os.path.exists('build/'):
        os.makedirs('build/')
    os.chdir('build/')

    #get root dir
    rootBuild = os.getcwd()
    ANDROID_TOOLCHAIN = rootSrc + "/" + ANDROID_TOOLCHAIN



    # check
    if (options.android is not None):
        crossAndroid(options.android, ('' if (options.androidAbi is None) else options.androidAbi))

    if (options.windows is not None):
        crossWindows()

    if (options.native is not None):
        native()

    #if (options.clean is not None):
        #cleanCmake()

    printCol("\n=============================================================", CYAN)
    if error:
        printCol("[__ALL__] ERROR", RED)
        exit(1)
    else:
        printCol("[__ALL__] SUCCESS", GREEN)


# ANDROID
def crossAndroid(api, abi):
    global packing, androidInstall, androidRun, clean

    if (abi == ''):
        abiList = ANDROID_ABIS
    else:
        abiList = [abi]

    printCol("\n=============================================================\n"
             "[ANDROID] API '%s'" % api, CYAN)

    okABIs = ''
    errABIS = ''

    for abi in abiList:
        chdir('cmake-android/' + abi)
        print("[ANDROID] build abi '%s'" % abi)
        run(['cmake',
             '-DCMAKE_TOOLCHAIN_FILE=' + ANDROID_TOOLCHAIN,
             '-DANDROID_NATIVE_API_LEVEL=' + api,
             '-DANDROID_APK_API_LEVEL=' + api,
             '-DANDROID_ABI=' + abi,
             '-DCMAKE_BUILD_TYPE=Debug',
             '' + rootSrc])
        ok = not call(['make'])#, '-j', '8'])
        if ok:
            okABIs += abi + ", "
        else:
            errABIS += abi + ", "



    okApk = False
    if (okABIs != ''):
        if (packing):
            printCol("\n[ANDROID] create apk", CYAN)

            if androidInstall:
                okApk = run(['make', 'InstallApk'])
            elif androidRun:
                okApk = run(['make', 'RunApk'])
            else:
                okApk = run(['make', 'BuildApk'])
        else:
            okApk = True
    else:
        okApk = True

    setError(okApk)
    print("\n[ANDROID] -- summary ----------------------------------")
    printOk("[ANDROID] " + ("SUCCESSFULLY build for '" + okABIs + "'" if (okABIs != '') else 'all builds ERROR'), okABIs != '')
    if errABIS != '':
        printCol("[ANDROID] " + "ERROR at build for '" + errABIS + "'", RED)
    if packing:
        printOk("[ANDROID] build APK %s " % ('SUCCESSFULLY' if okApk else 'ERROR'), okApk)



# WINDOWS
def crossWindows():
    chdir('cmake-windows')
    printCol("\n=============================================================\n"
             "[WINDOWS] windows cross build is not supported (will come later)", CYAN)


# NATIVE
def native():
    chdir('cmake-native')
    global packing
    cleanCmake()

    printCol("\n=============================================================\n"
             "[NATIVE] build", CYAN)

    run(['cmake',
         '' + rootSrc])
    okMake = run(['make'])
    setError(okMake)

    okPack = False
    if okMake:
        if packing:
            printCol("\n[NATIVE] create package", CYAN)
            okPack = run(['make', 'package'])
    #setError(okPack)

    print("\n[NATIVE] -- summary ----------------------------------")
    printOk("[NATIVE] build  '%s' " % ('SUCCESSFULLY' if okMake else 'ERROR'), okMake)
    printOk("[NATIVE] create package %s " % ('SUCCESSFULLY' if okPack else 'ERROR'), okPack)


def cleanCmake():
    if clean:
        printCol("\n=============================================================\n"
                 "[CLEAN] clean all cmake files in '" + os.getcwd() + "'", CYAN)

        for file in listFiles(['*.cmake', '*.txt', 'Makefile', 'CMakeFiles', '_CPack*', 'source']):
            if os.path.isfile(file):
                os.remove(file)
            elif os.path.isdir(file):
                shutil.rmtree(file)


# ## helper
def run(command):
    s = ""
    for a in command:
        s += a + " "
    printCol(s, BLUE)
    return not call(command)


def listFiles(listWildcard):
    files = []
    for w in listWildcard:
        files.extend(glob.glob(w))
    return files

def chdir(relativePath):
    global rootBuild
    if not os.path.exists(rootBuild + "/" + relativePath):
        os.makedirs(rootBuild +"/" + relativePath)
    os.chdir(rootBuild +"/" + relativePath)

def setError(ok):
    global error
    error |= not ok


# print color
RED = "\033[1;31m"
BLUE = "\033[1;34m"
CYAN = "\033[1;36m"
GREEN = "\033[0;32m"
RESET = "\033[0;0m"
BOLD = "\033[;1m"
REVERSE = "\033[;7m"


def printCol(text, color):
    # sys.stdout.write(color)
    print(color + text + RESET)
    # sys.stdout.write(RESET)


def printOk(text, ok):
    if ok:
        printCol(text, GREEN)
    else:
        printCol(text, RED)


if __name__ == "__main__":
    main(sys.argv[1:])
