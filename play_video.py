import os
import zlib
import cv2

def decompress_files(input_file):
    # Create the output directory if it doesn't exist
    output_dir = 'test_frames'
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    with open(input_file, 'rb') as f:
        file_counter = 1
        while True:
            # Read the compressed size of the next file
            compressed_size_bytes = f.read(4)
            if not compressed_size_bytes:
                break
            compressed_size = int.from_bytes(compressed_size_bytes, byteorder='little')

            # Read the compressed data
            compressed_data = f.read(compressed_size)

            # Decompress the data
            decompressed_data = zlib.decompress(compressed_data)

            # Write the decompressed data to a file in the output directory
            output_file_path = os.path.join(output_dir, f'{file_counter}.ppm')
            with open(output_file_path, 'wb') as outfile:
                outfile.write(decompressed_data)
            
            file_counter += 1

def play_video(frames_dir):
    frames = sorted(os.listdir(frames_dir))

    cap = cv2.VideoCapture()

    frame = cv2.imread(os.path.join(frames_dir, frames[0]))
    height, width, _ = frame.shape

    cv2.namedWindow('Video', cv2.WINDOW_NORMAL)
    cv2.resizeWindow('Video', width, height)

    # Read and display each frame
    for frame_name in frames:
        frame_path = os.path.join(frames_dir, frame_name)
        frame = cv2.imread(frame_path)
        cv2.imshow('Video', frame)
        cv2.waitKey(42)  # 24 fps video
    
   
    cap.release()
    cv2.destroyAllWindows()

# Decompress the files
input_file = 'video.vzip'
decompress_files(input_file)

# Play the video
frames_dir = 'test_frames'
play_video(frames_dir)
