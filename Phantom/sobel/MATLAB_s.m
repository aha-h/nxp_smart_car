clear all;
cam_h=60;
cam_w=94;

imag=imread('6.png');
%imag=rgb2gray(imag);


  for i=1:cam_h
		for j=1:cam_w
            imag1(i,j)=double(imag(i*6,j*7));
            imag2(i,j)=uint8 (imag(i,j));
        end
  end

subplot(1,3,1)
imshow(imag);

tempx=0.1;tempy=0.1;temp=0.1;
  for i=2:cam_h-1
		for j=2:cam_w-1				
				tempx=double((-  imag1(i-1,j-1))+(-2*imag1(i  ,j-1))+(-  imag1(i+1,j-1))+(   imag1(i-1,j+1))+( 2*imag1(i  ,j+1))+(   imag1(i+1,j+1)));
				if(tempx<0)
					tempx=-tempx;
				end
				tempy=double((   imag1(i+1,j-1))+( 2*imag1(i+1,j  ))+(   imag1(i+1,j+1))+(-  imag1(i-1,j-1))+(-2*imag1(i-1,j  ))+(-  imag1(i-1,j+1)));
				if(tempy<0)
					tempy=-tempy;
				end
				temp=tempx+tempy;
				if(temp>255)
					temp=255;
				end	
				imag2(i,j)=uint8(temp);	
                if(imag2(i,j)>240)
                    imag3(i,j)=255;
                    
                end
                if(imag2(i,j)<240)
                    imag3(i,j)=0;
                    
                end
		end
  end 
 
subplot(1,3,2)
imshow(imag2);

subplot(1,3,3)
imshow(imag3);
