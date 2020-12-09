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
