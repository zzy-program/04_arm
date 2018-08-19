function oim=show_enhanced_img( enhIm , orgIm)

Ybr = rgb2ycbcr(orgIm);
Ybr(:,:,1) = enhIm;
oim=ycbcr2rgb(Ybr);
imagesc([oim orgIm]);