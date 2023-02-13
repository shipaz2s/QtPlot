import cv2
import numpy as np
import os

def rotate(image, angle, center = None, scale = 1.0):
	(h, w) = image.shape[:2]

	if center is None:
		center = (w / 2, h / 2)

	M = cv2.getRotationMatrix2D(center, angle, scale)
	rotated = cv2.warpAffine(image, M, (w, h))

	return rotated

size = 512
thickness = 50
intend = thickness
bg_img = np.zeros((size, size, 4), np.uint8)

long_line = np.copy(bg_img)
short_line = np.copy(bg_img)
cv2.line(long_line, (intend, size//2), (size - intend, size // 2), (0, 0, 0, 255), thickness)
cv2.line(short_line, (intend, size//2),
	( intend + (size - 2 * intend) // 3, size // 2),
	(0, 0, 0, 255),
	thickness)

line_up = rotate(short_line, 45, (intend, size//2))
line_down = rotate(short_line, -45, (intend, size//2))
#left_arrow = line_up + line_down + long_line
left_arrow = np.bitwise_or(line_up, line_down)
left_arrow = np.bitwise_or(left_arrow, long_line)

right_arrow = cv2.flip(left_arrow, 1)
up_arrow = cv2.rotate(left_arrow, cv2.ROTATE_90_CLOCKWISE)
down_arrow = cv2.rotate(left_arrow, cv2.ROTATE_90_COUNTERCLOCKWISE)

left_right_arrow = np.bitwise_or(left_arrow, right_arrow)
up_down_arrow = np.bitwise_or(up_arrow, down_arrow)

thickness = thickness // 2
intend = thickness // 2
long_line = np.copy(bg_img)
short_line = np.copy(bg_img)
cv2.line(long_line, (intend, size//2), ( intend * 2 + (size - 2 * intend) // 4, size // 2), (0, 0, 0, 255), thickness)
cv2.line(short_line, (intend * 2, size//2),
	( intend * 2 + (size - 2 * intend) // 4, size // 2),
	(0, 0, 0, 255),
	thickness)
line_up = rotate(short_line, -45, (intend * 2 + (size - 2 * intend) // 4, size//2))
line_down = rotate(short_line, 45, (intend * 2 + (size - 2 * intend) // 4, size//2))
left_line = np.copy(bg_img)
right_line = np.copy(bg_img)
left_line = cv2.line(left_line, (intend * 2 + (size - 2 * intend) // 4 + thickness, intend * 2), (intend * 2 + (size - 2 * intend) // 4 + thickness, size - intend * 2), (0, 0, 0, 255), thickness*2 )

in_left_arrow = np.bitwise_or(line_up, line_down)
in_left_arrow = np.bitwise_or(in_left_arrow, long_line)
in_left_arrow = np.bitwise_or(in_left_arrow, left_line)
in_right_arrow = cv2.flip(in_left_arrow, 1)
in_left_right_arrow = np.bitwise_or(in_left_arrow, in_right_arrow)
in_up_down_arrow = cv2.rotate(in_left_right_arrow, cv2.ROTATE_90_CLOCKWISE)


prefix = ''
if '__file__' in locals():
	prefix = os.path.dirname(os.path.realpath(__file__)) + '/resources/'

cv2.imwrite(prefix + "left_arrow.png", left_arrow)
cv2.imwrite(prefix + "right_arrow.png", right_arrow)
cv2.imwrite(prefix + "left_right_arrow.png", left_right_arrow)
cv2.imwrite(prefix + "up_arrow.png", up_arrow)
cv2.imwrite(prefix + "down_arrow.png", down_arrow)
cv2.imwrite(prefix + "up_down_arrow.png", up_down_arrow)
cv2.imwrite(prefix + "in_left_right_arrow.png", in_left_right_arrow)
cv2.imwrite(prefix + "in_up_down_arrow.png", in_up_down_arrow)

#plt.imshow(in_up_down_arrow)
#plt.show()