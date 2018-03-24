function runSymbols = ivlc(vlcStream)
% IVLC - Inverse Variable Length Coding function receives as an input stream
% of VLC codes and creates the runSymbols that this stream came from.
% The encoding is using the codes included in the tables 2-D.15, 2-D.16 and
% 2-D.17 in the MPEG-1 documentation, which are global variables
% p. 92 in the documentation

%% Initialize
% Define the global variables to use them also in this file
getTheGlobals;
global d15a;
global d15b;
global d16a;
global d16b;
global d17a;
global d17b;


counter=1; % The length of the 2XR  matrix
sequence="";
escaped=0;
idle=0;
sign=0;

for i = 1 : strlength(vlcStream)
   
    if strlength(vlcStream(i:end)) <= 2  
        if vlcStream(i:end) == "10"
            return
        else
            disp('No EOB code found');
        end
    end
    
    sequence=sequence+vlcStream(i);
    
    if sign == 1
        if sequence == "1"
            runSymbols(counter-1,2)=runSymbols(counter-1,2)*(-1);
        end
        sign=0;
        sequence="";
        continue
    end
    
    if escaped==0
            if any(strcmp(d15b(:),sequence))

                if sequence == "000001" % Here I am about to jump to search in another table
                    escaped=1;
                    sequence="";
                else 
                    temp = find(strcmp(d15b(:),sequence),1);
                    runSymbols(counter,:)=d15a(temp,:);
                    counter=counter+1;
                    sequence="";
                    sign=1;
                end
            else 
                continue
            end
    else
        if any(strcmp(d16b(:),sequence)) && idle==0
            temp = find(strcmp(d16b(:),sequence),1);
            runSymbols(counter,1)=d16a(temp,1);
            sequence="";
            idle=1;
        elseif any(strcmp(d17b(:),sequence))
            temp = find(strcmp(d17b(:),sequence),1);
            runSymbols(counter,2)=d17a(temp,1);
            counter=counter+1;
            sequence="";
            escaped=0;
            idle=0;
        else
            continue
        end
    end
end

%% The following code can be used if the vlc stream is saved in a cell array, containing a string of a vlc code in each cell 
% 
% L = length(vlcStream);
% % Its size will change later, just keep the format of 1x2 array
% runSymbols = zeros(1,2);
% rsIndex = 1;
% 
% %% Scan the VLC code and change it to run Symbols
% for i = 1 : L
%     % Check if each VLC code exists in the d15b array - 1st case
%     % Isolate the code without the sign
%     vlc_mat = cell2mat(vlcStream(i));
%     without_sign = vlc_mat(1:end-1);
%     [exists15, location15] = ismember(without_sign, d15b);
%     if (exists15) % If is exists in the d15b table just place the values and go on
%         runSymbols(rsIndex, :) = d15a(location15, :);
%         if (vlc_mat(end) == '1') % Put the sign in the values if last bit is 1, otherwise do not change anything
%             runSymbols(rsIndex, 2) = runSymbols(rsIndex, 2) * (-1);
%         end
%         rsIndex = rsIndex + 1;
%     elseif (strcmp(vlc_mat, '10'))
%         % This means we found EOB, so function can return
%         return
%     else
%         if (strcmp(vlc_mat(1:6), '000001')) %this means that an escape sequence was used and we have to search the other tables
%             % check the first 6 digits, which are from the table d16b
%             [exists16, location16] = ismember(vlc_mat(7:12), d16b);
%             if (exists16) 
%                 runSymbols(rsIndex, 1) = d16a(location16); % Write the first column(RUN EOB) and then search the other array for the LEVEL value
%                 [exists17, location17] = ismember(vlc_mat(13:end), d17b);
%                 if (exists17)
%                     runSymbols(rsIndex, 2) = d17a(location17);
%                 end
%             end
%             rsIndex = rsIndex + 1;
%         end
%     end
% end

%% END