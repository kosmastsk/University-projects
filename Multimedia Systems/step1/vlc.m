function vlcStream = vlc(runSymbols)
% VLC - Variable Length Coding function receives as an input the run
% Symbols that were created in a previous function and returns an array of
% bits, encoded with variable length words
% The encoding is using the codes included in the tables 2-D.15, 2-D.16 and
% 2-D.17 in the MPEG-1 documentation, which are global variables
% p. 92 in the documentation

%% initialize
% vlcStream = '';
vlcStream = {};
getTheGlobals;

% Define the global variables to use them also in this file
global d15a;
global d15b;
global d16a;
global d16b;
global d17a;
global d17b;

%% Scan the symbols and change it to VLC codes
R = size(runSymbols, 1);
% R = length(runSymbols)
for i = 1 : R
    [Lia15, Loc15b] = ismember(abs(runSymbols(i, :)), d15a, 'rows');
    
    % If it exists, add it to the VLC stream, otherwise we need to check
    % the escape sequence
    if Lia15 ~= 0 
%         vlcStream = strcat(vlcStream, d15b(Loc15b));
        vlcStream(end + 1) = cellstr(d15b(Loc15b));
        % Check if is positive or negative, so that we add the s value in
        % the end of the stream
        % 0 means a positive level and 1 means a negative level
        if runSymbols(i, :) >= 0 % greater or equal to 0, because 0 should not affect neither the positives nor the negatives
            vlcStream(end) = strcat(vlcStream(end), '0');
        else
            vlcStream(end) = strcat(vlcStream(end), '1');
        end
    end
    % In case we cannot find it in the first table, we use the escape
    % sequence. So we append the escape code, then search for the run
    % length and then the level, and append all of these values to the
    % stream
    if Lia15 == 0
%         vlcStream = strcat(vlcStream(end), '000001'); % escape code
        vlcStream(end + 1) = cellstr('000001');
        [Lia16, Loc16] = ismember(abs(runSymbols(i, 1)), d16a, 'rows');
        if Lia16 ~= 0
            [Lia17, Loc17] = ismember(abs(runSymbols(i, 2)), d17a, 'rows');
            if Lia17 ~= 0
%                 vlcStream = strcat(vlcStream, d16b(Loc16));
%                 vlcStream(end) = cellstr(d16b(Loc16));
                vlcStream(end) = strcat(vlcStream(end), d16b(Loc16));
%                 vlcStream = strcat(vlcStream, d17b(Loc17));
%                 vlcStream(end+1) = cellstr(d17b(Loc17));
                vlcStream(end) = strcat(vlcStream(end), d17b(Loc17));
            end
        end
    end
end
% Add the EOB in the end of the stream to inform the decoder that there 
%are no more quantized coefficients in the current 8 by 8 block
% vlcStream = strcat(vlcStream, '10'); % EOB
vlcStream(end+1) = cellstr('10');

% If we want the result as a concentrated string
vlcStream = cell2mat(vlcStream);

end
%% END
