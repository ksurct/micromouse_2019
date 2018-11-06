classdef CalculateSpeed < matlab.System
    % Untitled Add summary here
    %
    % This template includes the minimum set of functions required
    % to define a System object with discrete state.
    
    % Public, Nontunable properties
    properties(Nontunable)
        % Gear Ratio
        gearRatio
        % Wheel Radius
        wheelRadius
        % Sample Time
        sampleTime
    end
    
    properties(DiscreteState)
        
    end
    
    % Pre-computed constants
    properties(Access = private)
        
    end
    
    methods(Access = protected)
        function setupImpl(obj)
            % Perform one-time calculations, such as computing constants
        end
        
        function speed = stepImpl(obj,ticks)
            % Implement algorithm. Calculate y as a function of input u and
            % discrete states.
            %
            % This function takes in the number of ticks since last call and uses
            % workspace variables to calculate the speed of the wheel in meters/second
            
            rotationalSpeed = (ticks / obj.gearRatio) / obj.sampleTime; % rad/second
            speed = rotationalSpeed * obj.wheelRadius; % meters/second
        end
        
        function resetImpl(obj)
            % Initialize / reset discrete-state properties
        end
    end
    
    methods (Static, Access = protected)
        function header = getHeaderImpl
            header = matlab.system.display.Header(mfilename('class'), ...
                'Title','Calculate Speed',...
                'Text','This function takes in the number of ticks since last call and the parameters provided below to calculate the speed of the wheel');
        end
    end
end
