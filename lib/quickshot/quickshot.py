class Shooter(object):
    """
    QuickShot library provides easy function for calculating shooter parameters based
    on given target. 
    """

    min_rpm = 0
    max_rpm = 0
    min_angle = 0
    max_angle = 0

    def __init__(self, min_rpm, max_rpm, min_angle, max_angle):
        self.min_rpm = min_rpm
        self.max_rpm = max_rpm
        self.min_angle = min_angle
        self.max_angle = max_angle


    def calc_params(self, y, speed, top_spin=0, back_spin=0):
        """
        Calc_params calculates the needed physical parameters for shooting the ball.

         Args:
            y: The target distance from the robot, in centimeters (cm), which needs to be a positive float. 
            speed: The desired speed of the ball (forward velocity), in meters per second (m/s), 
                which needs to be a positive float.
            top_spin: The top spin speed, in meters per second (m/s), which needs to be a non-negative float.
                Set both top_spin and back_spin to 0 to achieve no spin.
            back_spin: The back spin speed, in meters per second (m/s), which needs to be a non-negative float.
                Set both top_spin and back_spin to 0 to achieve no spin.

        Returns:
            params: The parameters to be used for the robot.
        """
        top_rpm = 0
        bottom_rpm = 0
        angle = 0

        return {"top_rpm": top_rpm, "bottom_rpm": bottom_rpm, "angle": angle}
    
    ## The actual code
    import math
class shooter():
    roller1_rpm = 0
    roller2_rpm = 0
    min_angle = 0
    max_angle = 0

    def __init__(self, roller1_rpm, roller2_rpm, min_angle, max_angle):
        self.roller1_rpm = roller1_rpm
        self.roller2_rpm = roller2_rpm
        self.min_angle = min_angle
        self.max_angle = max_angle

    def calc_params(self, location, speed, top_spin, back_spin):
        time=location/speed
        y_speed=time*9.8
        angle=math.degrees(math.atan(y_speed/speed))
        launch_speed=math.sqrt(y_speed*y_speed+speed*speed)

        if top_spin==0 and back_spin==0:
            rpm=launch_speed/9*math.pi #circumference of the roller
            roller1_rpm=rpm
            roller2_rpm=rpm
        elif top_spin>0:
            ball_rpm=top_spin/5.6*math.pi
            roller1_rpm=(launch_speed-ball_rpm*5.6)/9
            roller2_rpm=(2*launch_speed/9)-roller1_rpm
        else:
            ball_rpm=top_spin/5.6*math.pi
            roller2_rpm=(launch_speed-ball_rpm*5.6)/9
            roller1_rpm=(2*launch_speed/9)-roller2_rpm
        
        max_angle=angle
