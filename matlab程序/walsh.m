%Divide-and-conquer butterfly algorithm to compute the walsh spectra
%���㲼������f��walsh��
%����fΪά����2^n����������nΪ��������f�ı�Ԫ���� 
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
        
    