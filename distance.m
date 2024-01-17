function r=distance(params)
    global u t y h
    J=params(1); b=params(2); K=params(3);
    R=params(4); L=params(5);
    A=[-b/J K/J; -K/L -R/L];
    B=[0; 1/L];
    C=[1,0];
    D=0;
    pols=eig(A);
    if any(real(pols)>0);
        r=inf;
        return
    end
    sys=ss(A,B,C,D)
    sysd=c2d(sys,h);
    y_model=lsim(sysd,u);
hold off
plot(t,u,"Color",[0.61319563, 0.1824336, 0.2687872], "LineWidth", 1, "LineStyle", "--");
hold on
plot(t,y,"Color",[0.28708711, 0.32912157, 0.69034504], "LineWidth", 1)
plot(t,y_model,"Color",[0.33355526, 0.64107876, 0.80445484], "LineWidth", 1)

ylabel("Velocitat rotor (RPM)")
xlabel("Temps (s)")
title("Resposta del motor en llaç obert")
legend(["Referència", "Resposta model real", "Resposta model aproximat"], "Location", "southeast")
grid on;
drawnow();
    r=norm(y-y_model)
end