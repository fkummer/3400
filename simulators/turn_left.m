function [ ro,fws,turn ] = turn_left( robot_orient)
%turns robot right
       ro = mod(robot_orient+3,4);
       fws=0;
       turn=0;
end


