function [ ro,fws,turn ] = turn_right( robot_orient)
%turns robot right
       ro = mod(robot_orient+1,4);
       fws=0;
       turn=0;
end

