import sys

# Simulation constants
DIAL_SIZE = 100
START_POS = 50

# Global state to match the C++ logic
first_pw = 0
second_pw = 0

def process_rotation(current_pos: int, rotation_amount: int) -> int:
    """
    Updates the current_pos and returns the number of zero crossings.
    """
    global first_pw
    
    steps = abs(rotation_amount)
    start_position = current_pos
    zero_crossings = 0

    quotient = steps // DIAL_SIZE
    remainder = steps % DIAL_SIZE

    # Add a 0 cross for each full dial rotation
    zero_crossings += quotient

    # Calculate possible extra zero crossing for the partial rotation
    if rotation_amount < 0:  # Left rotation
        if start_position != 0 and start_position <= remainder:
            zero_crossings += 1
        
        # Python's % operator handles negative numbers correctly (circular)
        current_pos = (start_position - remainder) % DIAL_SIZE
    else:  # Right rotation
        if start_position != 0 and start_position + remainder >= DIAL_SIZE:
            zero_crossings += 1
        
        current_pos = (start_position + remainder) % DIAL_SIZE

    # Check if the dial stopped on zero
    if current_pos == 0:
        first_pw += 1
        
    return zero_crossings, current_pos

def solve(filename: str):
    global second_pw
    
    rotation_amounts = []
    
    # 1. Read file and parse rotations
    try:
        with open(filename, 'r') as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue
                
                # Convert 'L10' to -10 and 'R10' to 10
                amount = int(line[1:])
                if line[0] == 'L':
                    rotation_amounts.append(-amount)
                else:
                    rotation_amounts.append(amount)
    except FileNotFoundError:
        print(f"Error: File {filename} not found.")
        return

    # 2. Initialize state
    current_pos = START_POS
    total_crossings = 0

    # 3. Process rotations (equivalent to std::accumulate)
    for amount in rotation_amounts:
        crossings, current_pos = process_rotation(current_pos, amount)
        total_crossings += crossings
    
    second_pw = total_crossings

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python script.py <filename>")
    else:
        solve(sys.argv[1])
        print(f"First Password : {first_pw}")
        print(f"Second Password : {second_pw}")