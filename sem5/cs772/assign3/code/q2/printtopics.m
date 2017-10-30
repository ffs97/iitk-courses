function printtopics(V)
    WO=textread('vocab.txt','%s'); % list of all 2000 words
    W_outputN=20; % number of most prominent words to print for each topic
    [M,K]=size(V);
    Topics=cell(K,1);
    for k=1:K
        [WPsort TermIndex]=sort(V(:,k),'descend');
        str='';
        for index=1:W_outputN       
            str=[str ' ' WO{TermIndex(index)}];
        end
        Topics{k}=str; 
    end

    for k=1:K
        fprintf(Topics{k});
        fprintf('\n');
    end