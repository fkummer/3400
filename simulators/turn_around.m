function [ ro,fws,turn ] = turn_around( robot_orient)
%turns robot right
       ro = mod(robot_orient+2,4);
       fws=0;
       turn=0;
end

