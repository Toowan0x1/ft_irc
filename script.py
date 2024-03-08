import pygame
import subprocess
from datetime import datetime, timedelta
import os
import time

# Configuration constants
CONF_background_image = "b.jpg"
CONF_width, CONF_height = 5120, 2880
CONF_window_flags = pygame.FULLSCREEN | pygame.NOFRAME | pygame.HWSURFACE
CONF_exit_passwd = "walid2501"
CONF_max_stay_time = 60608

start_time = datetime.now()
pygame.init()
screen = pygame.display.set_mode((CONF_width, CONF_height), flags=CONF_window_flags)
running = True
window_size = pygame.display.get_window_size()
background_image = pygame.image.load(CONF_background_image)
background_image = pygame.transform.scale(background_image, window_size)

def lock_screen():
    # Here you can implement the code to lock the screen
    # For example, on macOS, you can use:
    os.system("pmset displaysleepnow")
    return True  # Return True to indicate screen has been locked

# Sleep for 3 seconds before entering the main loop
time.sleep(3)

while running:
    for event in pygame.event.get():
        if event.type == pygame.KEYDOWN or event.type == pygame.MOUSEMOTION:
            if lock_screen():
                running = False  # Exit the loop if screen is locked

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:  # Exit if ESC key is pressed
                running = False

    screen.blit(background_image, [0, 0])
    pygame.display.flip()

    time_now = datetime.now()
    if start_time + timedelta(seconds=CONF_max_stay_time) < time_now:
        running = False

pygame.quit()
