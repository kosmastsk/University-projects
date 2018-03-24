% TREELIST - Lists data in cell arrays and structs as ascii "tree"
%
% Version 1.0
%
% This functions lists the contents of structs, sub struct, cell arrays
% and sub cell array with chars: |-\ viewing the connection in the data.
% The main differents from the builtin DISP, DISPLAY is that this function
% lists all levels of sub cell arrays or struct in cells or structs.
%
% The syntax is similar to WHOS:
%
%   treelist varname1 varname2 .....
%
% (C) Copyright 2002 Peter Rydes�ter, GNU General Public License.
% 
%
% Se also:
%
%   WHOS, WHO, DISP, DISPLAY, CELL, STRUCT
%   
%
function treelist(varargin)
  for n=1:length(varargin),
    v=evalin('caller',varargin{n});
    treelistsub(v,varargin{n});
  end
  return;
  
function treelistsub(dt,name,level)
  if nargin<3, level='';  end
  if nargin<2, name=inputname(1); end

  if isnumeric(dt) | ischar(dt),
    if length(level)==0,
      ss=sprintf('%s',name);
    else
      ss=sprintf('%s-%s ',level,name);
    end
    lv=length(level)+20;
    if length(ss)<lv, ss(end+1:lv)='.'; end
    if isempty(dt),
      dtstr='[]';
    else
      dtstr=evalc('disp(dt)');
    end
    idx=[1 find(ss=='-')];
    ss2=ss;
    ss2(idx(end):end)=' ';
    fst=1;
    while length(deblank(dtstr))>0,
      [tok,dtstr]=strtok(dtstr,char(10));
      if length(tok)<79-length(ss),
	disp(sprintf('%s %s',ss,tok));
      else
	if fst==1,
	  disp(ss);
	end
	disp(tok);
      end
      ss=ss2;
      fst=0;
    end
  elseif isstruct(dt),
    fn=fieldnames(dt);
    if length(dt)==0,
      disp(sprintf('%s-%s Empty STRUCT',level,name));      
      return;
    end
    if length(dt)>1,
      disp(sprintf('%s-%s ',level,name))
      level(find(level=='\'))=' ';
      for m=1:length(dt),
	if m==length(dt),
	  treelistsub(dt(m),sprintf('%s(%d)',name,m),[level ' \']);
	else
	  treelistsub(dt(m),sprintf('%s(%d)',name,m),[level ' |']);
	end
      end
      return;
    end
    disp(sprintf('%s-%s',level,name))
    level(find(level=='\'))=' ';
    for n=1:length(fn),
      ww=warning;                      %%HACK To remove warning msg
      warning off;
      dts=getfield(dt,fn{n});
      if n==length(fn),
	treelistsub(dts,sprintf('%s',fn{n}),[level ' \']);
      else
	treelistsub(dts,sprintf('%s',fn{n}),[level ' |']);
      end
      warning(ww);
    end
    return;
  elseif iscell(dt),
    if length(dt)==0,
      disp(sprintf('%s-%s Empty CELL',level,name));      
      return;
    end
    disp(sprintf('%s-%s ',level,name))
    level(find(level=='\'))=' ';
    for m=1:length(dt),
      if m==length(dt),
	treelistsub(dt{m},sprintf('%s{%d}',name,m),[level ' \']);
      else
	treelistsub(dt{m},sprintf('%s{%d}',name,m),[level ' |']);
      end
    end
    return;
  end
  return;
  