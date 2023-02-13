#import numpy as np
#import cv2
#import matplotlib.pyplot as plt

img_size = 512
half_size = img_size // 2
bg_img = np.zeros(shape = (img_size,img_size,4), dtype=np.uint8)
width = 50

def create_arrow(arrow_width):
	left_arrow_img = np.copy(bg_img)
	for i in range( img_size // 3 ):
		for j in range(arrow_width // 2):
			di = int(i / 1.5)
			left_arrow_img[half_size + j - di][i][3] = 255
			left_arrow_img[half_size - j - di][i][3] = 255
			left_arrow_img[half_size + j + di][i][3] = 255
			left_arrow_img[half_size - j + di][i][3] = 255

	center_y = img_size // 3 - arrow_width // 2
	center_x = half_size - int(center_y / 1.5)
	rx = arrow_width / 2.17
	ry = arrow_width / 2.17

	for x in range(arrow_width):
		for y in range(arrow_width):
			if (x*x / (rx*rx) + y*y / (ry*ry) ) > 1.:# and y <= center_y:
				left_arrow_img[center_x + x][center_y + y][3] = 0
				left_arrow_img[center_x - x][center_y + y][3] = 0

	center_y = img_size // 3 - arrow_width // 2
	center_x = half_size + int(center_y / 1.5)

	for x in range(arrow_width):
		for y in range(arrow_width):
			if (x*x / (rx*rx) + y*y / (ry*ry) ) > 1.:# and y <= center_y:
				left_arrow_img[center_x + x][center_y + y][3] = 0
				left_arrow_img[center_x - x][center_y + y][3] = 0
	return left_arrow_img

####################### начинаем стрелки наружу ##############################
left_arrow_img = create_arrow(width)

################## преобразуем в стрелки внутрь, сразу дорисуем ########################
save_width = width
width = width
left_arrow_tmp = create_arrow(width)

center_y = img_size // 3 - width // 4 * 3
center_x = half_size - int(center_y / 1.5)
rx = width / 2.17
ry = width / 2.17

for x in range(width):
	for y in range(width):
		if (x*x / (rx*rx) + y*y / (ry*ry) ) > 1.:# and y <= center_y:
			left_arrow_tmp[center_x + x][center_y + y][3] = 0
			left_arrow_tmp[center_x - x][center_y + y][3] = 0

center_y = img_size // 3 - width // 4 * 3
center_x = half_size + int(center_y / 1.5)

for x in range(width):
	for y in range(width):
		if (x*x / (rx*rx) + y*y / (ry*ry) ) > 1.:# and y <= center_y:
			left_arrow_tmp[center_x + x][center_y + y][3] = 0
			left_arrow_tmp[center_x - x][center_y + y][3] = 0

slice_for_invert = left_arrow_tmp[:,:img_size // 3,:]
slice_for_invert = np.fliplr(slice_for_invert)

in_left_arrow_img = np.copy(bg_img)
in_left_arrow_img[:,:img_size // 3,:] = slice_for_invert

for i in range(0, img_size // 3):
	for j in range(width // 2):
		in_left_arrow_img[half_size + j][i][3] = 255
		in_left_arrow_img[half_size - j][i][3] = 255

for i in range( half_size ):
	for j in range(width):
		in_left_arrow_img[half_size + i][img_size // 3 + j][3] = 255
		in_left_arrow_img[half_size - i][img_size // 3 + j][3] = 255

in_right_arrow_img = np.fliplr(in_left_arrow_img)
in_left_right_arrow_img = in_right_arrow_img + in_left_arrow_img
in_up_down_arrow_img = np.transpose(in_left_right_arrow_img, (1,0,2))

####################### дорисуем стрелки наружу ##############################
save_width = save_width
for i in range(img_size):
	for j in range(width // 2):
		left_arrow_img[half_size + j][i][3] = 255
		left_arrow_img[half_size - j][i][3] = 255

r = width // 2
center_x = half_size
center_y = r

rx = width / 1.25
ry = width / 2


for x in range(width):
	for y in range(width):
		if (x*x / (rx*rx) + y*y / (ry*ry) ) > 1. and y <= center_y:
			left_arrow_img[center_x - x][center_y - y][3] = 0
			left_arrow_img[center_x + x][center_y - y][3] = 0

right_arrow_img = np.fliplr(left_arrow_img)

left_mask = np.copy(bg_img)

for i in range(img_size):
	for j in range(img_size // 2):
		left_mask[i][j][3] = 255

left_mask = left_arrow_img & left_mask
right_mask = np.fliplr(left_mask)


left_right_arrow_img = right_mask + left_mask
up_down_arrow_img = np.transpose(left_right_arrow_img, (1,0,2))
up_arrow_img = np.transpose(left_arrow_img, (1,0,2))
down_arrow_img = np.flip(up_arrow_img, 0)

cv2.imwrite("left_arrow.png", left_arrow_img)
cv2.imwrite("right_arrow.png", right_arrow_img)
cv2.imwrite("left_right_arrow.png", left_right_arrow_img)
cv2.imwrite("up_arrow.png", up_arrow_img)
cv2.imwrite("down_arrow.png", down_arrow_img)
cv2.imwrite("up_down_arrow.png", up_down_arrow_img)
cv2.imwrite("in_left_right_arrow.png", in_left_right_arrow_img)
cv2.imwrite("in_up_down_arrow.png", in_up_down_arrow_img)

#plt.imshow(up_down_arrow_img)
#plt.show()