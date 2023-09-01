import pygame
from pygame.locals import *
from pygame.math import Vector3 as Vec3
import sys
pygame.init()


def get_values(file):
    with open(file, 'r') as f:
        lines = f.readlines()
    
    vts = []
    fs = []
    for line in lines:
        split = line.strip('\n').strip().split(" ")
        if split[0] == "vt":
            vt = split[1:]
            x = float(vt[0])
            y = float(vt[1])
            vts.append([x, 1.0 - y])
        elif split[0] == "f":
            f = split[1:]
            fs.append(f)
    
    return vts, fs

def draw(screen, image, fs, vts):
    screen.blit(image, (0, 0))
    image_width = image.get_width()
    image_height = image.get_height()
    for f in fs:
        points = []
        for indexes in f:
            values = indexes.split("/")
            vt_index = int(values[1])
            points.append(vts[vt_index - 1])
        
        points = [[int(image_width * point[0]), int(image_height * point[1])] for point in points]

        for i in range(len(points) - 2):
            p = [points[0], points[1 + i], points[2 + i]]
            side_1 = Vec3(p[1][0], p[1][1], 0) - Vec3(p[0][0], p[0][1], 0)
            side_2 = Vec3(p[2][0], p[2][1], 0) - Vec3(p[1][0], p[1][1], 0)
            
            color = (0, 0, 0)
            if pygame.Vector3.cross(side_1, side_2).z < 0:
                # color = (255, 0, 0)
                temp = p[1]
                p[1] = p[2]
                p[2] = temp
                side_1 = Vec3(p[1][0], p[1][1], 0) - Vec3(p[0][0], p[0][1], 0)
                side_2 = Vec3(p[2][0], p[2][1], 0) - Vec3(p[1][0], p[1][1], 0)
                if pygame.Vector3.cross(side_1, side_2).z < 0:
                    color = (255, 0, 0)
                else:
                    color = (0, 255, 0)
            else:
                color = (0, 255, 0)
            
            pygame.draw.polygon(screen, color, p, 1)


if __name__ == '__main__':
    obj = "log"
    texture = "log"
    vts, fs = get_values(f"build/files/objs/{obj}.obj")
    image = pygame.image.load(f"build/files/images/{texture}.png")
    scale_factor = 0.1
    image = pygame.transform.scale(image, (image.get_width() * scale_factor, image.get_height() * scale_factor))
    screen = pygame.display.set_mode(image.get_size())
    draw(screen, image, fs, vts)
    pygame.display.update()
    while True:
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == KEYDOWN and event.key == K_ESCAPE:
                pygame.quit()
                sys.exit()