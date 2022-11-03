%Divide-and-conquer butterfly algorithm to compute the walsh spectra
%计算布尔函数f的walsh谱
%输入f为维数是2^n的行向量，n为布尔函数f的变元个数 
function spectra = walsh(f,n);
% a=f';
a=1-2*f';
for i=1:n
    t=2^n/2^i;
    for j=0:2:2^i-1
        a(t*j+1:t*(j+1),1)=a(t*j+1:t*(j+1),1)+a(t*(j+1)+1:t*(j+2),1);
        a(t*(j+1)+1:t*(j+2),1)=a(t*j+1:t*(j+1),1)-2*a(t*(j+1)+1:t*(j+2),1);
    end
end
spectra=a;
        
    