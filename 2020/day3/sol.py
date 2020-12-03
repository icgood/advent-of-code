import sys

grid = []
for line in sys.stdin:
    grid.append(line.rstrip())
grid_width = len(grid[0])
grid_height = len(grid)


def is_tree(x: int, y: int) -> bool:
    x %= grid_width
    return grid[y][x] == '#'


def num_trees(slope_x: int, slope_y: int) -> int:
    x, y, trees = 0, 0, 0
    while y < grid_height:
        if is_tree(x, y):
            trees += 1
        x += slope_x
        y += slope_y
    return trees


# part 1
print('part 1:', num_trees(3, 1))


# part 2
slopes = [(1, 1), (3, 1), (5, 1), (7, 1), (1, 2)]
multiplied = 1
for slope_x, slope_y in slopes:
    multiplied *= num_trees(slope_x, slope_y)
print('part 2:', multiplied)
