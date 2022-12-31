import random

NO_OF_ROWS = 10

def generate_locations():
    for i in range(0,NO_OF_ROWS):
        print(f'"{random.uniform(0.0,1000.0)},{random.uniform(0.0,1000.0)},{random.uniform(0.0,1000.0)},{random.uniform(-180.0,180.0)}"')

generate_locations()
