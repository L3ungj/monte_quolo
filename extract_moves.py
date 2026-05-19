# extracts moves from barricade.gg HTML

import re

with open('initial_moves.html', 'r') as f:
    html = f.read()
moves = re.findall(r'>([vh]?[a-i][1-9])<', html)

print(moves)

with open('initial_moves.txt', 'w') as f:
    f.write('\n'.join(moves))