import cv2

def main():
    camera = cv2.VideoCapture(0)  

    if not camera.isOpened():
        print("camear is not open.")
        return

    while True:
        ret, frame = camera.read()

        if not ret:
            print("camera is not open.")
            break

        cv2.imshow("frame", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    camera.release()
    cv2.destroyAllWindows()
 image = camera.read_image()
 cv2.imwrite('image.png', image)
    
if __name__ == "__main__":
    main()
