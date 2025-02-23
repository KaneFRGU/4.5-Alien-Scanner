import pygame
import math
import datetime
import random
from smbus import SMBus
 
addr = 0x8 # bus address
bus = SMBus(11) # indicates /dev/ic2-1

width, height = 800, 480

pygame.init()
screen = pygame.display.set_mode((width, height))
clock = pygame.time.Clock()

backgroundImg = pygame.image.load("Circle.png")
bg = pygame.image.load("BG.png")
pointSprite = pygame.image.load("point.png")
noise1 = pygame.image.load("noise1.png")
noise2 = pygame.image.load("noise2.png")
noise3 = pygame.image.load("noise3.png")
noise4 = pygame.image.load("noise4.png")

noises = [noise1, noise2, noise3, noise4]

max_distance = 100

font = pygame.font.Font("font.otf", 32)
points = []


class point:
    def __init__(self, dst, angle):
        self.dst = dst
        self.angle = angle
        self.time = 2
        self.fadeAnimation = 1

def addPoint(points, dst,angle):
    if(angle > 135):
        angle = 135
    if(angle < 45):
        angle = 45
    if(dst < 0):
        dst = 0
    if(dst > max_distance):
        dst = max_distance

    points.append(point(dst,angle))
    return points


def getClosestPoint(points):
    closest = max_distance

    for point in points :
        if point.dst < closest:
            closest = point.dst

    return closest

def getFurthestPoint(points):
    furthest = 0

    for point in points :
        if point.dst > furthest:
            furthest = point.dst

    return furthest

points = addPoint(points, 60, 70)

dt = 0
getLastTicks = 0
dataTimer = 0

running = True
while running:
    t = pygame.time.get_ticks()
    dt = (t - getLastTicks) / 1000.0
    getLastTicks = t

    for event in pygame.event.get():  
        if event.type == pygame.QUIT:
            running = False

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False

    dataTimer+= dt

    if dataTimer >= 0.3:
        data = bus.read_i2c_block_data(addr,0, 2)
        if data[0] != 0:
            points = addPoint(points, data[0], data[1]+45)

        dataTimer = 0

    screen.fill((255,255,255))
    screen.blit(bg, (0,0))
    screen.blit(backgroundImg, (0,0))

    index = 0
    for i in range(len(points)):
        if points[index ].fadeAnimation <= 0:
            points.pop(index)
            index-=1

        index+=1

    for i in range(len(points)):
        angle = points[i].angle
        rad = math.radians(angle)

        if(points[i].dst == 0):
            y = height
        else:
            y = height - (height / (max_distance / points[i].dst))
        
        points[i].time -=1 * dt
        
        if(points[i].time <= 0):
            points[i].fadeAnimation -= 1 * dt

        if(points[i].fadeAnimation <= 0):
            pointSprite.set_alpha(0)
        else:
            pointSprite.set_alpha(256 * (points[i].fadeAnimation / 1))

        x = points[i].dst * 3.3 * math.cos(rad) + 400
        y = height - points[i].dst * math.sin(rad)  * 3.7 - 60


        screen.blit(pointSprite, (x-24, y-24))



    if len(points) >0:
        text = font.render("Closest: " +str(getClosestPoint(points)) + "cm", True, (151,255,15))
        textRect = text.get_rect()
        textRect.x = width - textRect.w - 5
        textRect.y = height - 68  

        screen.blit(text, textRect)

        text = font.render("Furthest: " +str(getFurthestPoint(points)) + "cm", True, (151,255,15))
        textRect = text.get_rect()
        textRect.x = width - textRect.w - 5
        textRect.y = height - 38 

        screen.blit(text, textRect)

        text = font.render("Total: " + str(len(points)), True, (151,255,15))
        textRect = text.get_rect()
        textRect.x = width - textRect.w - 5
        textRect.y = height - 98 

        screen.blit(text, textRect)



    screen.blit(noises[random.randrange(0,3)], (0,0))

    clock.tick(30)
    pygame.display.flip()