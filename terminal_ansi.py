import sys
import enum


class Terminal(enum.Enum):
    Inline      = "\r\033[2K" # \r moves to the start, \033[2K clears the line
    Default     = "\033[0m"
    Green       = "\033[92m"
    Blue        = "\033[34m"
    Red         = "\033[31m"

    Bold        = "\033[1m"
    BoldGreen   = "\033[1;92m"
    BoldBlue    = "\033[1;34m"
    BoldRed     = "\033[1;31m"

    def __str__(self):
        # Colors are disabled in non-TTY environments such as pipes. This means
        # that if output is redirected to a file, it will not contain color codes
        if sys.stdout.isatty():
            return self.value
        else:
            return ''
    
    def __call__(self, *values):
        if self is Terminal.Inline:
            print(self, *' '.join(values), "\033[0m", sep="", end="", flush=True)
        else:
            print(self, *' '.join(values), "\033[0m", sep="", flush=True)
