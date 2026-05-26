import sys

def main():
    # Check if a filename was provided
    if len(sys.argv) != 2:
        print("Usage: python word_count.py <file name>")
        sys.exit(1)
    
    try:
        # Read the entire file content
        with open(sys.argv[1], 'r') as file:
            file_string = file.read()
        
        # Split the content into words using the same delimiters as the Java version
        import re
        words = re.split("[ \n\t\r.,;:!?(){}]", file_string)
        
        # Create a dictionary to store word counts (equivalent to TreeMap in Java)
        # Using a regular dict since we'll sort the output at the end
        word_count = {}
        
        # Count the frequency of each word
        for word in words:
            # Convert to lowercase and check if it's not empty
            key = word.lower()
            if key:  # This checks if the string is not empty
                if key in word_count:
                    word_count[key] += 1
                else:
                    word_count[key] = 1
        
        # Print the results in alphabetical order
        for key in sorted(word_count.keys()):
            print(f"{key}\t {word_count[key]}")
            
    except FileNotFoundError:
        print(f"Error: File '{sys.argv[1]}' not found")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
