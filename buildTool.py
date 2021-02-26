import os

def stagePrint(txt, brlen=25):
    print("{br} {txt} {br}".format(br='='*brlen, txt=txt))


def _getChar():
    try:
        import msvcrt
        return msvcrt.getch

    except ImportError:
        import sys, tty, termios
        def _getchUnix():
            fd = sys.stdin.fileno()
            old_settings = termios.tcgetattr(fd)
            try:
                tty.setraw(fd)
                ch = sys.stdin.read(1)
            finally:
                termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
            return ch

        return _getchUnix

getCharSys = _getChar()

def getChar(txt):
    print(txt, end="")
    char = str(getCharSys())
    if(ord(char) >= 48 and ord(char) <= 126):
        print(char)
    return char

def quitMenu():
    print("\nGoodbye!")
    exit(0)


# ========================= Compiler Menu ============================

def compilerBuild():
    os.system("make -C ./SilentCompiler")

def compilerBuildDev():
    os.system("make CFLAGS=-g -C ./SilentCompiler")

def compilerTest():
    stagePrint("Debugging Silent Compiler")
    os.system("./SilentCompiler/out/SilentC")
    stagePrint("Silent Compiler Exit")

def compilerClean():
    os.system("make clean -C ./SilentCompiler")


def compilerMenu():
    menuOptions = {
        "1": compilerBuild,
        "2": compilerBuildDev,
        "3": compilerTest,
        "4": compilerClean,
        "9": lambda: os.system("clear"),
        "0": lambda: os.system("reset")
    }

    running = True
    while running:
        print("\n")
        stagePrint("Silent Compiler")
        print("1. Build Prod")
        print("2. Build Debug")
        print("3. Debug Build")
        print("4. Clean")
        print()
        print("9. Console Clear")
        print("0. Console Reset")
        print("backspace. back")
        print("esc. quit")
        print("\n")

        option = getChar(":")
        
        if(option in menuOptions.keys()):
            menuOptions.get(option)()
        elif(ord(option) == 27): #esc
            quitMenu()
        elif(ord(option) == 127): #backspace
            return
            


# ========================= VM Menu ============================

def vmMenu():
    pass



# ========================= Main Menu ============================

def main():
    menuOptions = {
        "1": compilerMenu,
        "2": vmMenu,
        "9": lambda: os.system("clear"),
        "0": lambda: os.system("reset")
    }

    running = True
    while running:
        print("\n")
        stagePrint("Silent Build Tool")
        print("1. Silent Compiler")
        print("2. Silent VM")
        print()
        print("9. Console Clear")
        print("0. Console Reset")
        print("esc. Quit")
        print("\n")

        option = getChar(":")
        
        if(option in menuOptions.keys()):
            menuOptions.get(option)()
        elif(ord(option) == 27): #esc
            quitMenu()



if __name__ == "__main__":
    main()