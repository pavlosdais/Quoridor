#! /usr/bin/env python2

import sys
import os
import subprocess
import signal
import atexit
from time import time
from getopt import getopt
from select import select

EMPTY = 0
WHITE = 1
BLACK = 2

noneORIENTATION = 0
HORIZONTAL = 1
VERTICAL = 2


class Wall(object):
    x = 0                           # wall's x coordinate
    y = 0                           # wall's y coordinate
    orientation = noneORIENTATION   # walls's orientation

    def __init__(self, x, y, orientation):
        self.x = x
        self.y = y
        self.orientation = orientation


class Player(object):
    path = ""                           # file path
    name = None                         # name command's response
    subprocess = None                   # player's subprocess
    color = EMPTY                       # player's color
    walls = 0                           # walls available of each player
    x = 0                               # player's x coordinate
    y = 0                               # player's y coordinate
    gamesWon = 0                        # how many games the player has won
    time_left = 0                       # time left for the player to complete all his moves
    last_command = ""                   # last message sent to player's stdin
    running = False                     # subprocess running

    def __init__(self, color):
        self.color = color

    def readName(self):
        self.write("name")
        s = self.read()
        if s is None:
            self.timeOut("response")
            return
        if s.startswith("="):
            self.name = s[1:].strip()
        else:
            unexpectedOutput(self, s)
            self.name=self.path

    def run(self):
        try:
            self.subprocess = subprocess.Popen(["./"+self.path, "-seed", str(seed)], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, bufsize=0)
        except:
            print("Could not execute %s, quiting..." % self.getTitle())
            sys.exit(4)
        self.running = True
        self.readName()

    def kill(self):
        try:
            os.kill(self.subprocess.pid, signal.SIGINT)
        except:
            print("Could not kill %s" % self.getTitle())
        self.running = False

    def winner(self):
        if (self.color==BLACK and self.y==board.size-1) or (self.color==WHITE and self.y==0):
            return True
        return False

    def memory(self):
        return int(os.popen("ps -o vsz %d | tail -n 1" % self.subprocess.pid).readline())

    def getTitle(self):
        if self.color==BLACK:
            color="black"
        else:
            color="white"
        if self.name is None:
            title = self.path
        else:
            title = self.name
        return "%s (%s player)" % (title, color)

    def write(self, s):
        try:
            self.subprocess.stdin.write((s+'\n').encode("utf-8"))
            self.subprocess.stdin.flush()
            self.last_command = s
        except:
            notResponding(self)

    def timeOut(self, s):
        if verbose>0:
            print("time out waiting for %s from %s after command: \"%s\"" % (s, self.getTitle(), self.last_command))

    def read(self, timeout=2):
        start_time = time()
        try:
            if select([self.subprocess.stdout], [], [], timeout)[0]:
                s=self.subprocess.stdout.readline().decode("utf-8").strip()
            else: # timeout
                return None
            while s=="" or s.startswith('#'):
                if s.startswith('#') and verbose>0:
                    print("%s comments: %s"%(self.getTitle(), s))
                if select([self.subprocess.stdout], [], [], timeout-(time()-start_time))[0]:
                    s=self.subprocess.stdout.readline().decode("utf-8").strip()
                else: # timeout
                    return None
            return s
        except:
            notResponding(self)

    def readOK(self):
        s = self.read()
        while s!="=":
            if s is None:
                self.timeOut("'='")
                return 0
            unexpectedOutput(self, s)
            s = self.read()
        return 1

    def readMove(self, timeout):
        start_time = time()
        line = self.read(timeout)
        if line is None:  # timeout
            return None
        while (not line.startswith("=")) or len(line)<3:
            unexpectedOutput(self, line)
            line = self.read(timeout-(time()-start_time))
            if line is None:  # timeout
                return None
        s = line[1:].strip().split()
        t = board.stringToCoords(s[0])
        if not t:
            unexpectedOutput(self, line)
            return self.readMove(timeout-(time()-start_time))
        x, y=t
        if len(s)==1:  # move
            return noneORIENTATION, x, y
        else:  # wall
            s=s[1].lower()
            if s=="h" or s=="horizontal":
                orientation = HORIZONTAL
            elif s=="v" or s=="vertical":
                orientation = VERTICAL
            else:
                unexpectedOutput(self, line)
                return self.readMove(timeout-(time()-start_time))
            return orientation, x, y

    def readWinner(self):
        while True:
            line = self.read()
            if line is None:
                self.timeOut("response")
                return EMPTY
            if line.startswith("?"):
                unexpectedOutput(self, line)
                continue
            s = line[1:].strip().lower()
            if s=="true white":
                return WHITE
            elif s=="true black":
                return BLACK
            elif s=="false":
                return EMPTY
            else:
                unexpectedOutput(self, line)


class Board(object):
    size = 9                            # board's size - default 9
    walls = []                          # list of walls on board
    player_1 = Player(BLACK)            # black player
    player_2 = Player(WHITE)            # white player

    def coordsToString(self, x, y):
        return "%c%d" % (ord("A")+x, self.size-y)

    def stringToCoords(self, s):
        try:
            x = ord(s[0].lower())-ord('a')
            y = self.size-int(s[1:3])
            return x, y
        except:
            return None

    def orientationToString(self, orientation):
        if orientation==HORIZONTAL:
            return "horizontal"
        elif orientation==VERTICAL:
            return "vertical"
        else:
            return "unknown orientation"

    def wallExists(self, x, y, orientation):
        for wall in self.walls:
            if wall.x==x and wall.y==y and wall.orientation==orientation:
                return True
        return False

    def print_board(self):
        sys.stdout.write("\n     ")
        for j in range(0, self.size):
            sys.stdout.write("%c   " % (ord('A')+j))
        print("")
        for i in range(0, self.size):
            sys.stdout.write("   ")
            for j in range(0, self.size):
                if self.wallExists(j-1, i-1, HORIZONTAL):
                    sys.stdout.write('=')
                elif self.wallExists(j-1, i-1, VERTICAL):
                    sys.stdout.write('H')
                else:
                    sys.stdout.write('+')
                if self.wallExists(j-1, i-1, HORIZONTAL) or self.wallExists(j, i-1, HORIZONTAL):
                    sys.stdout.write("===")
                else:
                    sys.stdout.write("---")
            print("+")
            sys.stdout.write("%2d " % (self.size-i))
            for j in range(0, self.size):
                if self.wallExists(j-1, i-1, VERTICAL) or self.wallExists(j-1, i, VERTICAL):
                    sys.stdout.write('H')
                else:
                    sys.stdout.write('|')
                if self.player_1.x==j and self.player_1.y==i:
                    sys.stdout.write(" B ")
                elif self.player_2.x==j and self.player_2.y==i:
                    sys.stdout.write(" W ")
                else:
                    sys.stdout.write("   ")
            sys.stdout.write("| %d" % (self.size-i))
            if i==0:
                print("  black walls: %2d" % self.player_1.walls)
            elif i==1:
                print("  white walls: %2d" % self.player_2.walls)
            else:
                print("")
        sys.stdout.write("   ")
        for j in range(0, self.size):
            sys.stdout.write("+---")
        sys.stdout.write("+\n     ")
        for j in range(0, self.size):
            sys.stdout.write("%c   " % (ord('A')+j))
        print("\n")

    def thereIsWall(self, p_x, p_y, x, y):
        if p_x==x:
            m = min(p_y, y)
            M = max(p_y, y)
            for i in range(m, M):
                if self.wallExists(p_x, i, HORIZONTAL) or (p_x>0 and self.wallExists(p_x-1, i, HORIZONTAL)):
                    return True
        elif p_y==y:
            m = min(p_x, x)
            M = max(p_x, x)
            for i in range(m, M):
                if self.wallExists(i, p_y, VERTICAL) or (p_y>0 and self.wallExists(i, p_y-1, VERTICAL)):
                    return True
        else:
            return True
        return False

    def checkValidMove(self, x, y, player):
        if x<0 or y<0 or x>=self.size or y>=self.size:
            return False
        if (self.player_1.x==x and self.player_1.y==y) or (self.player_2.x==x and self.player_2.y==y):
            return False
        distance = abs(player.x-x)+abs(player.y-y)
        if distance==1:
            if self.thereIsWall(player.x, player.y, x, y):
                return False
            return True
        elif distance==2 and (player.x==x or player.y==y):
            x_middle = (player.x+x)/2
            y_middle = (player.y+y)/2
            if not ((self.player_1.x==x_middle and self.player_1.y==y_middle) or (self.player_2.x==x_middle and self.player_2.y==y_middle)):
                return False
            if self.thereIsWall(player.x, player.y, x, y):
                return False
            return True
        elif distance==2:
            if player.color==BLACK:
                opponent=self.player_2
            else:
                opponent=self.player_1
            if (abs(player.x-opponent.x)+abs(player.y-opponent.y))!=1:
                return False
            xBehind=player.x
            yBehind=player.y
            if player.x+1==opponent.x:
                xBehind=player.x+2
            elif player.x-1==opponent.x:
                xBehind=player.x-2
            elif player.y+1==opponent.y:
                yBehind=player.y+2
            elif player.y-1==opponent.y:
                yBehind=player.y-2
            else:
                return False
            if (xBehind < 0 or xBehind >= self.size or yBehind < 0 or yBehind >= self.size or self.thereIsWall(opponent.x, opponent.y, xBehind, yBehind)) and not self.thereIsWall(player.x, player.y, opponent.x, opponent.y) and not self.thereIsWall(opponent.x, opponent.y, x, y):
                return True
        return False

    def playmove(self, x, y, player):
        if self.checkValidMove(x, y, player)!=1:
            return False
        player.x = x
        player.y = y
        return True

    def tryToReachOpponentSide(self, player, tmpBoard):
        if player.winner():
            return True
        x = player.x
        y = player.y
        tmpBoard[x][y] = 1
        if player.color==BLACK:
            sign=1
        else:
            sign=-1
        if self.playmove(x, y+sign, player):
            if not tmpBoard[x][y+sign] and self.tryToReachOpponentSide(player, tmpBoard):
                return True
        player.y=y
        if self.playmove(x+1, y, player):
            if not tmpBoard[x+1][y] and self.tryToReachOpponentSide(player, tmpBoard):
                return True
        player.x=x
        if self.playmove(x-1, y, player):
            if not tmpBoard[x-1][y] and self.tryToReachOpponentSide(player, tmpBoard):
                return True
        player.x=x
        if self.playmove(x, y-sign, player):
            if not tmpBoard[x][y-sign] and self.tryToReachOpponentSide(player, tmpBoard):
                return True
        player.y=y
        return False

    def checkValidBoard(self):
        tmpBoard = [[0 for i in range(self.size)] for j in range(self.size)]
        XsYs = self.player_1.x, self.player_1.y, self.player_2.x, self.player_2.y
        self.player_2.x = -1
        self.player_2.y = -1
        if not self.tryToReachOpponentSide(self.player_1, tmpBoard):
            self.player_1.x, self.player_1.y, self.player_2.x, self.player_2.y = XsYs
            return False
        tmpBoard = [[0 for i in range(self.size)] for j in range(self.size)]
        _, _, self.player_2.x, self.player_2.y = XsYs
        self.player_1.x = -1
        self.player_1.y = -1
        if not self.tryToReachOpponentSide(self.player_2, tmpBoard):
            self.player_1.x, self.player_1.y, self.player_2.x, self.player_2.y = XsYs
            return False
        self.player_1.x, self.player_1.y, self.player_2.x, self.player_2.y = XsYs
        return True

    def checkValidWall(self, x, y, orientation):
        if x<0 or y<0 or x>self.size-2 or y>self.size-2:
            return False
        for wall in self.walls:
            if ((wall.x==x and wall.y==y) or
                (wall.x-1==x and wall.y==y and wall.orientation==orientation and orientation==HORIZONTAL) or
                (wall.x+1==x and wall.y==y and wall.orientation==orientation and orientation==HORIZONTAL) or
                (wall.x==x and wall.y-1==y and wall.orientation==orientation and orientation==VERTICAL) or
                (wall.x==x and wall.y+1==y and wall.orientation==orientation and orientation==VERTICAL)):
                return False
        return True

    def playwall(self, x, y, orientation, player):
        if player.walls <= 0 or not self.checkValidWall(x, y, orientation):
            return False
        self.walls.append(Wall(x, y, orientation))
        if self.checkValidBoard():
            player.walls -= 1
            return True
        else:
            self.walls.pop()
        return False


move_time = 30.0                    # allowed maximum time for a movement - default 30 seconds
verbose = 0                         # default 0, 1 to list moves, 2 to draw board
games = 1                           # how many games to play - default 1
gameCounter = 0
seed = int(time())                  # seed to pass to players - default current unix timestamp
starting_walls = -1                 # how many walls each player has available at the start of the game - default(-1) 7/4*boardsize-23/4
maximum_time = -1                   # maximum number of seconds players can consume for all their moves - default(-1) 20*boardsize
memory_limit = 950                  # memomy limit of player's subrocesses, if zero not checking memory usage - default 950MB
helpstring = """

Run with:

quoridor_referee.py --white \'<path to program 1>\' \\
                    --black \'<path to program 2>\' \\

Possible quoridor_referee options:

  --verbose 1 (to list moves) or --verbose 2 (to draw board)
  --size <board size>               (default 9)
  --walls <number of walls>         (default 7/4*boardsize-23/4)
  --games <number of games to play> (default 1)
  --maximum_time <time(in seconds)> (default 20*boardsize)
  --move_time <time(in seconds)>    (default 30)
  --memory_limit <memory(in MB)>    (default 950MB)
  --seed <seed>                     (default current unix timestamp)

"""


def usage():
    print(helpstring)
    sys.exit(2)


board = Board()


def notResponding(player):
    print("\n%s is not responding, quiting..." % player.getTitle())
    sys.exit(6)


def invalidMove(color, x, y, orientation):
    if color==BLACK:
        winner=board.player_2
        loser=board.player_1
    else:
        winner=board.player_1
        loser=board.player_2
    if orientation:
        move = "%s %s" % (board.coordsToString(x,y), board.orientationToString(orientation))
    else:
        move = "%s" % board.coordsToString(x,y)
    print("\n%s tried an invalid move: %s" % (loser.getTitle(), move))
    print("%s wins game %d\n" % (winner.getTitle(), gameCounter+1))
    print("--------------------")


def memoryExceed(color):
    if color==BLACK:
        winner=board.player_2
        loser=board.player_1
    else:
        winner=board.player_1
        loser=board.player_2
    print("\n%s exceeded the memory limit" % loser.getTitle())
    print("%s wins game %d\n" % (winner.getTitle(), gameCounter+1))
    print("--------------------")
    loser.kill()


def timeOut(color):
    if color==BLACK:
        winner=board.player_2
        loser=board.player_1
    else:
        winner=board.player_1
        loser=board.player_2
    print("\n%s ran out of time" % loser.getTitle())
    print("%s wins game %d\n" % (winner.getTitle(), gameCounter+1))
    print("--------------------")
    loser.kill()


def unexpectedOutput(player, s):
    if(verbose>0):
        print("unexpected output from %s: \"%s\" after command: \"%s\"" % (player.getTitle(), s, player.last_command))


def winner(player):
    print("\n%s wins game %d" % (player.getTitle(), gameCounter+1))
    board.player_1.write("winner")
    w1 = board.player_1.readWinner()
    board.player_2.write("winner")
    w2 = board.player_2.readWinner()
    if w1 == w2 == player.color:
        print("both players agree")
    else:
        if w1 != player.color:
            print("%s disagrees" % board.player_1.getTitle())
        else:
            print("%s agrees" % board.player_1.getTitle())
        if w2 != player.color:
            print("%s disagrees" % board.player_2.getTitle())
        else:
            print("%s agrees" % board.player_2.getTitle())
    print("%s took %.2fs to decide its moves"%(board.player_1.getTitle(), maximum_time-board.player_1.time_left))
    print("%s took %.2fs to decide its moves"%(board.player_2.getTitle(), maximum_time-board.player_2.time_left))
    print("\n--------------------")


def initialize_game():
    if not board.player_1.running:
        board.player_1.run()
    if not board.player_2.running:
        board.player_2.run()
    board.walls = []
    board.player_1.walls = starting_walls
    board.player_2.walls = starting_walls
    board.player_1.time_left = maximum_time
    board.player_2.time_left = maximum_time
    board.player_1.y = int(0)
    board.player_1.x = int(board.size/2)
    board.player_2.y = int(board.size-1)
    board.player_2.x = int(board.size/2)
    board.player_1.write("boardsize %d" % board.size)
    board.player_1.readOK()
    board.player_1.write("clear_board")
    board.player_1.readOK()
    board.player_1.write("walls %d" % starting_walls)
    board.player_1.readOK()
    board.player_2.write("boardsize %d" % board.size)
    board.player_2.readOK()
    board.player_2.write("clear_board")
    board.player_2.readOK()
    board.player_2.write("walls %d" % starting_walls)
    board.player_2.readOK()


def playGame():
    initialize_game()
    print("\n\nGame %d starting now!\n" % (1+gameCounter))
    if verbose>1:
        board.print_board()
    while True:
        if memory_limit and board.player_2.memory()>memory_limit*1024:  # checking memory for white
            memoryExceed(WHITE)
            return BLACK
        board.player_1.write("genmove black")
        start_time = time()
        move = board.player_1.readMove(min(move_time, board.player_1.time_left))
        board.player_1.time_left -= time()-start_time
        if move: #no timeout
            if move[0]:  # wall
                if not board.playwall(move[1], move[2], move[0], board.player_1):
                    invalidMove(BLACK, move[1], move[2], move[0])
                    return WHITE
                board.player_2.write("playwall black %s %s" % (board.coordsToString(move[1], move[2]), board.orientationToString(move[0])))
                if verbose>0:
                    print("%s places %s wall at %s" % (board.player_1.getTitle(), board.orientationToString(move[0]), board.coordsToString(move[1], move[2])))
            else:  # move
                if not board.playmove(move[1], move[2], board.player_1):
                    invalidMove(BLACK, move[1], move[2], move[0])
                    return WHITE
                board.player_2.write("playmove black %s" % board.coordsToString(move[1], move[2]))
                if verbose>0:
                    print("%s moves to %s" % (board.player_1.getTitle(), board.coordsToString(move[1], move[2])))
            board.player_2.readOK()
            if verbose>1:
                board.print_board()
            if board.player_1.winner():
                winner(board.player_1)
                return BLACK
        else:
            timeOut(BLACK)
            return WHITE

        if memory_limit and board.player_1.memory()>memory_limit*1024:  #chevking memory for black
            memoryExceed(BLACK)
            return WHITE
        board.player_2.write("genmove white")
        start_time = time()
        move = board.player_2.readMove(min(move_time, board.player_2.time_left))
        board.player_2.time_left -= time()-start_time
        if move:  # no timeout
            if move[0]:  # wall
                if not board.playwall(move[1], move[2], move[0], board.player_2):
                    invalidMove(WHITE, move[1], move[2], move[0])
                    return BLACK
                board.player_1.write("playwall white %s %s" % (board.coordsToString(move[1], move[2]), board.orientationToString(move[0])))
                if verbose>0:
                    print("%s places %s wall at %s" % (board.player_2.getTitle(), board.orientationToString(move[0]), board.coordsToString(move[1], move[2])))
            else:  # move
                if not board.playmove(move[1], move[2], board.player_2):
                    invalidMove(WHITE, move[1], move[2], move[0])
                    return BLACK
                board.player_1.write("playmove white %s" % board.coordsToString(move[1], move[2]))
                if verbose>0:
                    print("%s moves to %s" % (board.player_2.getTitle(), board.coordsToString(move[1], move[2])))
            board.player_1.readOK()
            if(verbose>1):
                board.print_board()
            if board.player_2.winner():
                winner(board.player_2)
                return WHITE
        else:
            timeOut(WHITE)
            return BLACK


####intro####

print("Welcome to the Quoridor Tournament! I'm going to be your referee for this game")
print("Created by Teo Georgiou for the Introduction to Programming course, UoA, 2011")
print("Updated by John Vlachopoulos and Sotirios-Efstathios Maneas for the Introduction to Programming course, UoA, 2012-2013")
print("Updated by Spyros Avlonitis and Giannos Chatziagapis for the Introduction to Programming course, UoA, 2015-2021")
print("\n--------------------\n\n")

####read parameters####
try:
    (opts, params) = getopt(sys.argv[1:], "", ["black=",
                                               "white=",
                                               "size=",
                                               "walls=",
                                               "verbose=",
                                               "games=",
                                               "maximum_time=",
                                               "move_time=",
                                               "memory_limit=",
                                               "seed="])
    for opt, value in opts:
        if opt == "--black":
            board.player_1.path = value
        elif opt == "--white":
            board.player_2.path = value
        elif opt == "--size":
            board.size = int(value)
        elif opt == "--walls":
            starting_walls = int(value)
        elif opt == "--verbose":
            verbose = int(value)
        elif opt == "--games":
            games = int(value)
        elif opt == "--maximum_time":
            maximum_time = int(value)
        elif opt == "--move_time":
            move_time = float(value)
        elif opt == "--memory_limit":
            memory_limit = int(value)
        elif opt == "--seed":
            seed = int(value)

except:
    usage()

if board.player_1.path == "" or board.player_2.path == "":
    usage()

if maximum_time<0: # default=20*boardsize
    maximum_time=board.size*20
if starting_walls<0: # default=7/4*boardsize-23/4
    starting_walls=int(7.0/4*board.size-23.0/4)

if board.size<3 or board.size>25 or board.size%2==0 or starting_walls<0 or games<=0 or maximum_time<=0 or move_time<=0:
    usage()

print("boardsize set to %d" % board.size)
print("walls set to %d" % starting_walls)
print("verbose set to %d" % verbose)
print("games set to %d" % games)
print("maximum_time set to %d" % maximum_time)
print("move_time set to %.1f" % move_time)
if memory_limit:
    print("memory_limit set to %d" % memory_limit)
print("seed set to %d" % seed)

####check files exist and are executable####

while not os.path.exists(board.player_1.path):
    print("File %s does not exist, please specify the filename correctly" % board.player_1.path)
    board.player_1.path = input('> ')
if not os.access(board.player_1.path, os.X_OK):
    print("File %s is not executable, quiting..." % board.player_1.path)
    sys.exit(3)

while not os.path.exists(board.player_2.path):
    print("File %s does not exist, please specify the filename correctly" % board.player_2.path)
    board.player_2.path = input('> ')
if not os.access(board.player_2.path, os.X_OK):
    print("File %s is not executable, quiting..." % board.player_2.path)
    sys.exit(3)

####start subprocesses####

board.player_1.run()
board.player_2.run()


####at exit####

def cleanup():
    board.player_1.kill()
    board.player_2.kill()


atexit.register(cleanup)

####Save game script####

f = open("last_game.sh","w")
f.write("#!/bin/bash\n\n")
if "--seed" not in sys.argv:
    sys.argv.append("--seed %d"%seed)
f.write(" ".join(sys.argv))
f.write("\n")
f.close()
mode = os.stat("last_game.sh").st_mode
mode |= (mode & 0o444) >> 2
os.chmod("last_game.sh", mode)

####Games starting####

print("\n--------------------")
print("%s vs %s" % (board.player_1.getTitle(), board.player_2.getTitle()))
print("----------------------")


while gameCounter < games:
    i = playGame()
    if i==BLACK:
        board.player_1.gamesWon += 1
    else:
        board.player_2.gamesWon += 1
    gameCounter+=1
    # if abs(board.player_1.gamesWon-board.player_2.gamesWon)>games-gameCounter:
    #     break

if games > 1:
    print("%s won %d games" % (board.player_1.getTitle(), board.player_1.gamesWon))
    print("%s won %d games" % (board.player_2.getTitle(), board.player_2.gamesWon))
    if board.player_1.gamesWon > board.player_2.gamesWon:
        print("\n%s wins!!" % board.player_1.getTitle())
    elif board.player_1.gamesWon < board.player_2.gamesWon:
        print("\n%s wins!!" % board.player_2.getTitle())
    else:
        print("\nIt's a tie!!")
    print("--------------------\n")
