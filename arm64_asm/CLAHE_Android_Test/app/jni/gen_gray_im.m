function gen_gray_im( im, filename)

Ybr = rgb2ycbcr(im);

f=fopen(filename,'w');
fwrite(f,Ybr(:,:,1)','uint8');
fclose(f);