%gives the polynomial of f.
function value=truthtable2polynomial(f,n)
ANF=mod(abs(NNF(f,n)),2);
B=de2bi((0:2^n-1)','left-msb',n);
X=flip(sym('x',[1 n]));
polynomial=0;
for weight=0:n
    for i=1:2^n
        if sum(B(i,1:n))==weight && ANF(i,1)==1
            polynomial=polynomial+prod(X.^B(i,1:n));
        end
    end
end
value=polynomial;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Gao Cheng
% % fΪ2^nά������
% %��һ������ʱ��Ҫ��������������硣
% 
% for i=1:n
%  syms (['x_',num2str(i)]);
% end  %�������
% 
% %******�����Ҫ���nֵ����Ӧ���������Ƶ����
% 
% %for i=1:2^n-1
% %    xx(1,i)=eval(['x_',int2str(i) ,';']);
% %end  
% 
% if n==2
%     xx=[x_1;x_2];
% end
% if n==3
%     xx=[x_1;x_2;x_3];
% end
% if n==4
%     xx=[x_1;x_2;x_3;x_4];
% end
% if n==5
%     xx=[x_1;x_2;x_3;x_4;x_5];
% end
% if n==6
%     xx=[x_1;x_2;x_3;x_4;x_5;x_6];
% end
% if n==7
%     xx=[x_1;x_2;x_3;x_4;x_5;x_6;x_7];
% end
% if n==8
%     xx=[x_1;x_2;x_3;x_4;x_5;x_6;x_7;x_8];
% end
% if n==9
%     xx=[x_1;x_2;x_3;x_4;x_5;x_6;x_7;x_8;x_9];
% end
% if n==10
%     xx=[x_1;x_2;x_3;x_4;x_5;x_6;x_7;x_8;x_9;x_10];
% end
% if n==11
%     xx=[x_1;x_2;x_3;x_4;x_5;x_6;x_7;x_8;x_9;x_10;x_11];
% end
% if n==12
%     xx=[x_1;x_2;x_3;x_4;x_5;x_6;x_7;x_8;x_9;x_10;x_11;x_12];
% end
% if n==13
%     xx=[x_1;x_2;x_3;x_4;x_5;x_6;x_7;x_8;x_9;x_10;x_11;x_12;x_13];
% end
% %********************************
% value=0;%��ʼ��Ϊ0    
% for i=0:2^n-1
%     X=de2bi(i,n);
%     s=1;
%     for j=1:size(X,2)
%         s=s*(xx(j)+1+X(j));
%     end
%     value=value+f(i+1)*s;
% end
% value=simplify(value);
% value=mod(value,2);
% %������ɽ���*�����Ϊ�� �����ߡ�\cdot ��������Latex��༭��ɡ�
