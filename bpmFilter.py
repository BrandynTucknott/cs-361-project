"""
Microservice for Filtering Songs by BPM Range

This microservice continuously monitors a text file (request_pipeline.txt) for requests. Each request should consist
of two numbers separated by whitespace, representing the inclusive bounds of a bpm (beats per minute) range. Upon
receiving a valid request, the microservice processes the spotify-2023.csv file to identify songs within the specified
bpm range. The row numbers of these songs are then output to another text file (response_pipeline.txt) followed by a whitespace. Once the
response is generated, the request file is cleared, and the microservice resumes waiting for new requests.

Usage:
Run the script in a Python environment in parallel with main. Ensure that spotify-2023.csv is in the same directory or adjust the file path accordingly.

Author:
Jose Baroza-Martinez

"""

import csv
import time

def read_bpm_range_from_file(file_path):
    with open(file_path, 'r') as file:                # open request file
        line = file.readline().strip()                # read first line
        if line:                                      # check if line is not empty
            lower_bpm, upper_bpm = map(int, line.split())  # parse bpm range
            return lower_bpm, upper_bpm
        else:
            return None, None                         # return None if file is empty

def find_songs_in_bpm_range(csv_file_path, lower_bpm, upper_bpm):
    row_numbers = []                                 # initialize list for row numbers
    bpm_column_index = 14                            # assuming bpm is column 15

    try:
        with open(csv_file_path, newline='', encoding='ISO-8859-1') as csvfile:  # open csv with specified encoding
            reader = csv.reader(csvfile)
            next(reader)                             # skip header row
            for row_number, row in enumerate(reader, start=2):  # iterate through csv rows
                try:
                    bpm = int(row[bpm_column_index])  # get bpm value
                    if lower_bpm <= bpm <= upper_bpm:  # check if bpm is in range
                        row_numbers.append(row_number)  # add row number to list
                except ValueError:
                    # handle non-integer bpm values
                    continue
    except UnicodeDecodeError:
        # handle file decoding errors
        print("Error: Unable to decode file. Please check the file encoding.")

    return row_numbers

def write_results_to_file(file_path, row_numbers):
    with open(file_path, 'w') as file:                # open file for writing
        for number in row_numbers:
            file.write(f'{number}\n')                 # write each row number

def clear_file(file_path):
    open(file_path, 'w').close()                      # clear file contents

def main():
    # File paths
    request_file = 'request_pipeline.txt'
    response_file = 'response_pipeline.txt'
    csv_file = 'spotify-2023.csv'

    while True:                                      # continuously check for requests
        lower_bpm, upper_bpm = read_bpm_range_from_file(request_file)  # read request
        if lower_bpm is not None and upper_bpm is not None:
            rows = find_songs_in_bpm_range(csv_file, lower_bpm, upper_bpm)  # find songs in bpm range
            write_results_to_file(response_file, rows)  # write results to response file
            clear_file(request_file)                   # clear request file
            print(f'Processed BPM range {lower_bpm}-{upper_bpm} and updated {response_file}')
        time.sleep(0.5)                                # wait for 0.5 seconds before next check

if __name__ == "__main__":
    main()
