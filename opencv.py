import cv2

img = cv2.cvtColor(cv2.imread("data/girl.jpg"), cv2.COLOR_BGR2GRAY)
cv_res = cv2.Canny(img, threshold1=40, threshold2=160)
cv2.imshow("result of opencv", cv_res)
cv2.waitKey(0)
cv2.imwrite("data/girl_cv_result.jpg", cv_res)