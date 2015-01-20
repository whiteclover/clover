from solo.web.app import App 

class LoginApp(App):


    def initialize(self):
        route = self.route()
        route.mapper.explicit = False

        from clover.controller.user import UserMenu
        from clover.controller.guild import GuildMenu
        
        UserMenu(route)
        GuildMenu(route)
