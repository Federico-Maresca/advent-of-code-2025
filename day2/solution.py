import sys

# Global variables to track sums
first_invalid_id_sum = 0
second_invalid_id_sum = 0

def check_for_invalid_id(id_val: int):
    """
    Checks if an ID is 'invalid' based on repeating patterns.
    - first_invalid_id_sum: IDs repeating exactly twice (e.g., 12341234)
    - second_invalid_id_sum: IDs repeating 2 or more times (e.g., 111, 121212)
    """
    global first_invalid_id_sum, second_invalid_id_sum
    
    id_str = str(id_val)
    length = len(id_str)
    
    # Check for repeating patterns by dividing the length
    # dividend is the number of times the substring repeats
    for dividend in range(2, (length // 2) + 2):
        if length % dividend == 0:
            part_len = length // dividend
            part = id_str[:part_len]
            
            # Leading zeros do not count as valid parts
            if part[0] == '0':
                continue
            
            # Python string multiplication: '123' * 3 = '123123123'
            if part * dividend == id_str:
                if dividend == 2:
                    first_invalid_id_sum += id_val
                
                second_invalid_id_sum += id_val
                return

    # Special check for single digit repeated length times (e.g., 1111)
    # This covers cases where dividend > (length/2)+1 (like 111 where dividend is 3)
    if length > 1 and id_str[0] != '0':
        if all(char == id_str[0] for char in id_str):
            second_invalid_id_sum += id_val

def solve(id_ranges):
    for start, end in id_ranges:
        for id_val in range(start, end + 1):
            check_for_invalid_id(id_val)

def read_ranges(filename):
    id_ranges = []
    try:
        with open(filename, 'r') as f:
            # File contains ranges separated by commas, e.g., "100-200,500-600"
            content = f.read()
            parts = content.split(',')
            for part in parts:
                part = part.strip()
                if '-' in part:
                    print(f"Processing line: {part}")
                    start_str, end_str = part.split('-')
                    id_ranges.append((int(start_str), int(end_str)))
    except FileNotFoundError:
        print(f"Error: File {filename} not found.")
    return id_ranges

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python script.py <filename>")
    else:
        ranges = read_ranges(sys.argv[1])
        for r in ranges:
            print(f"Read range: {r[0]}-{r[1]}")
            
        solve(ranges)
        
        print(f"Invalid ranges (Exact twice) : {first_invalid_id_sum}")
        print(f"Invalid ranges (Total invalid) : {second_invalid_id_sum}")