import filecmp
import os

# Compare files in test_frames and frames
def compare_files(frames_dir, test_frames_dir):
    
    frames_files = sorted(os.listdir(frames_dir))
    test_frames_files = sorted(os.listdir(test_frames_dir))

    if len(frames_files) != len(test_frames_files):
        print("Number of files in 'frames' and 'test_frames' directories don't match.")
        return

    
    for frame_file in frames_files:
        # Extract numeric part from frame_file
        frame_file_num = int(frame_file.split('.')[0])  
        test_frame_file = f"{frame_file_num}.ppm"  

        frame_file_path = os.path.join(frames_dir, frame_file)
        test_frame_file_path = os.path.join(test_frames_dir, test_frame_file)
        
        if not filecmp.cmp(frame_file_path, test_frame_file_path):
            print(f"Files {frame_file} and {test_frame_file} are different.")
        else:
            print(f"Files {frame_file} and {test_frame_file} are identical.")

# Compare the files
frames_dir = 'frames'
test_frames_dir = 'test_frames'
compare_files(frames_dir, test_frames_dir)

