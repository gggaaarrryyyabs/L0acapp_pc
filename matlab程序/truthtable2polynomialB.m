%% [0 0 0 1]=1--x_1
function value = truthtable2polynomialB(f,n)
ANF=mod(abs(NNF(f,n)),2);
p=0; k=0;
B=find(ANF==1)-1; 
C=cell(length(B),1);
for i=0:n
    for j=1:length(B)
        b=de2bi(B(j,1),n);
        if sum(b')==i
            k=k+1;
            C{k,1}=num2str(find(b==1));
        end
    end
end
value=C;

% fid=fopen('zfai.txt','w');%建立文件
% fprintf(fid,'%s\n',aa{:,:});
% fclose(fid);


