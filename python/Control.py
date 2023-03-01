
import pygame
import json, os
import bluetooth

# Bluetooth device address
bd_addr = "fc:a8:9a:00:92:43" # PS4 MAC ADDRESS

# Port number to use for connection
port = 1

# Create a socket and connect to the device
sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
sock.connect((bd_addr, port))
sock.baudrate = 9600
################################# LOAD UP A BASIC WINDOW #################################
pygame.init()

running = True
LEFT, RIGHT, UP, DOWN , WATER , STOP_WATER ,LED , STOP_LED= False, False, False, False , False , False , False , False
LEFT_S, RIGHT_S= False, False
SPEED_L3=0
clock = pygame.time.Clock()
color = 0
###########################################################################################

#Initialize controller
joysticks = []
for i in range(pygame.joystick.get_count()):
    joysticks.append(pygame.joystick.Joystick(i))
for joystick in joysticks:
    joystick.init()

with open(os.path.join("ps4_keys.json"), 'r+') as file:
    button_keys = json.load(file)
# 0: Left analog horizonal, 1: Left Analog Vertical, 2: Right Analog Horizontal
# 3: Right Analog Vertical 4: Left Trigger, 5: Right Trigger
analog_keys = {0:0, 1:0, 2:0, 3:0, 4:-1, 5: -1 }
w=0
s=0
d=0
a=50
i=0
j=0
step_STR="000"
DATA_SEND="U000000"
# START OF GAME LOOP
while running:
    SPEED_R2 = "{:03d}".format(int(0))
    ################################# CHECK PLAYER INPUT #################################
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN:
            ############### UPDATE SPRITE IF SPACE IS PRESSED #################################
            pass

        # HANDLES BUTTON PRESSES
        if event.type == pygame.JOYBUTTONDOWN:
            if event.button == button_keys['left_arrow']:
                LEFT_S = True
                if (a < 100):
                    a+=1
                SPEED_L3 = "{:03d}".format(int(a))

                print("LEFT PRESS")
            if event.button == button_keys['right_arrow']:
                RIGHT_S = True
                if(a>20):
                    a-=1
                SPEED_L3 = "{:03d}".format(int(a))
                print("RIGHT PRESS")
            if event.button == button_keys['down_arrow']:
                DOWN = True
                print("DOWN PRESS")
            if event.button == button_keys['x']:
                WATER = True
                print(("WATER"))
            if event.button == button_keys['circle']:
                STOP_WATER = True
                print(("STOP WATER "))
            if event.button == button_keys['triangle']:
                LED = True
                print(("LED"))
            if event.button == button_keys['square']:
                STOP_LED = True
                print(("STOP LED "))
            if event.button == button_keys['up_arrow']:
                UP = True
                print("UP PRESS")
            #print(pygame.KEYDOWN)

        # HANDLES BUTTON RELEASES
        if event.type == pygame.JOYBUTTONUP:
            if event.button == button_keys['left_arrow']:
                print("LEFT RELESE")
            if event.button == button_keys['right_arrow']:
                RIGHT = False
                print("LEFT RELESE")
            if event.button == button_keys['down_arrow']:
                DOWN = False
                print("LEFT RELESE")
            if event.button == button_keys['up_arrow']:
                UP = False
                print("LEFT RELESE")
            if event.button == button_keys['x']:
                WATER = False
            if event.button == button_keys['circle']:
                STOP_WATER = False
            if event.button == button_keys['triangle']:
                LED = False
            if event.button == button_keys['square']:
                STOP_LED = False

        #HANDLES ANALOG INPUTS
        if event.type == pygame.JOYAXISMOTION:
            analog_keys[event.axis] = event.value
            # Horizontal Analog

            if abs(analog_keys[0]*5000) > 600:
                if analog_keys[0]*5000 < -3500:
                    LEFT_S = True
                    #print("L3 LEFT",128+analog_keys[0]*128)
                    #SPEED_L3 = "{:03d}".format(int(a))
                    #print(SPEED_L3)
                else:
                    LEFT = False
                if analog_keys[0]*5000 > 3500:
                    RIGHT_S = True
                    #print("L3 RIGHT",128+analog_keys[0]*128)
                    #SPEED_L3 = "{:03d}".format(int(a))
                    #print(SPEED_L3)

                else:
                    RIGHT_S = False

            if abs(analog_keys[2] * 5000) > 600:
                if analog_keys[2] * 5000 < -3500:
                    LEFT = True
                    #print("R3 LEFT", analog_keys[2] * 5000)
                else:
                    LEFT = False
                if analog_keys[2] * 5000 > 3500:
                    RIGHT = True
                    #print("R3 RIGHT", analog_keys[2] * 5000)
                else:
                    RIGHT = False
            # Vertical Analog
            if abs(analog_keys[3] * 5000) > 600:
                if analog_keys[3] * 5000 < -3500:
                    UP = True
                    #print("R3 UP", analog_keys[3] * 5000)
                else:
                    UP = False
                if analog_keys[3] * 5000 > 3500:
                    DOWN = True
                    #print("R3 DOWN", analog_keys[3] * 5000)
                else:
                    DOWN = False

                # Triggers
            if (128)+analog_keys[4] * 128 > 128:  # Left trigger
                #print("L2 DOWN ","{:03d}".format(int(analog_keys[4] * 256)))
                SPEED_L2="{:03d}".format(int(analog_keys[4] * 256))
                #print(SPEED_L2)
            if (128)+analog_keys[5] * 128 > 128:  # Right trigger
                #print("R2 DOWN ","{:03d}".format(int(analog_keys[5] * 256)))
                SPEED_R2="{:03d}".format(int((analog_keys[5] * 256))-1)
                #print(SPEED_R2)
            with open("steper.txt", "r") as file:
                step_STR=(file.read())
                if step_STR:
                        step_STR="{:03d}".format((85-int(step_STR)//3))
                        step_BOOL=True
                        #print(step_STR)
                else:
                    step_BOOL = False

            if(LEFT|RIGHT|UP|DOWN|LEFT_S|RIGHT_S|step_BOOL | WATER| STOP_WATER |LED |STOP_LED):
                if (LEFT):
                    DATA_SEND=("L"+str(SPEED_R2)+str(step_STR))
                elif (RIGHT):
                    DATA_SEND = ("R"+str(SPEED_R2)+str(step_STR))
                elif (UP):
                    DATA_SEND = ("U"+str(SPEED_R2)+str(step_STR))
                elif (DOWN):
                    DATA_SEND = ("D"+str(SPEED_R2)+str(step_STR))
                elif (LEFT_S):
                    DATA_SEND = ("Z"+str(SPEED_R2)+str(step_STR))
                elif (RIGHT_S):
                    DATA_SEND = ("X"+str(SPEED_R2)+str(step_STR))
                elif (step_BOOL):
                    DATA_SEND = ("X"+str(SPEED_R2)+str(step_STR))
                if (WATER):
                    DATA_SEND = ("W"+str(SPEED_R2)+str(step_STR))
                if (STOP_WATER):
                    DATA_SEND = ("R"+str(SPEED_R2)+str(step_STR))
                if (LED):
                    DATA_SEND = ("J"+str(SPEED_R2)+str(step_STR))
                if (STOP_LED):
                    DATA_SEND = ("K"+str(SPEED_R2)+str(step_STR))
                j+=1
                if(j>100):
                    print(DATA_SEND)
                    sock.send(DATA_SEND.encode())
                    j=0
            else:
                i+=1
                if(i>100):
                    DATA_SEND = ("U000"+str(step_STR))
                    sock.send(DATA_SEND.encode())
                    print(DATA_SEND)
                    i=0
