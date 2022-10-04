def printColor(text: str, foreground: str, background = "", sep=" ", end="\n") -> None:
    BackColors = {
        "black": "\u001b[40m",
        "red": "\u001b[41m",
        "green": "\u001b[42m",
        "yellow": "\u001b[43m",
        "blue": "\u001b[44m",
        "magenta": "\u001b[45m",
        "cyan": "\u001b[46m",
        "white": "\u001b[47m",
        "bblack": "\u001b[40;1m",
        "bred": "\u001b[41;1m",
        "bgreen": "\u001b[42;1m",
        "byellow": "\u001b[43;1m",
        "bblue": "\u001b[44;1m",
        "bmagenta": "\u001b[45;1m",
        "bcyan": "\u001b[46;1m",
        "bwhite": "\u001b[47;1m",
        "": ""
    }

    ForeColors = {
        "black": "\u001b[30m",
        "red": "\u001b[31m",
        "green": "\u001b[32m",
        "yellow": "\u001b[33m",
        "blue": "\u001b[34m",
        "magenta": "\u001b[35m",
        "cyan": "\u001b[36m",
        "white": "\u001b[37m",
        "reset": "\u001b[0m",
        "bblack": "\u001b[30;1m",
        "bred": "\u001b[31;1m",
        "bgreen": "\u001b[32;1m",
        "byellow": "\u001b[33;1m",
        "bblue": "\u001b[34;1m",
        "bmagenta": "\u001b[35;1m",
        "bbyan": "\u001b[36;1m",
        "bwhite": "\u001b[37;1m",
        "": ""
    }

    print(ForeColors[foreground] + BackColors[background] + text + ForeColors["reset"], sep=sep, end=end)