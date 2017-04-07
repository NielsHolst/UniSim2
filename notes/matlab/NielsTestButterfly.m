%test script for niels holst
%
%Oliver Körner, 31-03-2017

%% Import data
filename = 'butterfly_0030.txt';
startRow = 2;
endRow = inf;

[date1,Station,Tmin,Tmax,Tavg,I,eggcontent,eggoutflowTotal,larvacontent,larvaoutflowTotal,pupacontent,pupaoutflowTotal,adultcontent,adultoutflowTotal] = ...
    ImportButterfly(filename, startRow, endRow);

%day of year
doy=datenum(date1)-datenum('2009/1/1');

%% PLOT1
data={'eggcontent' 'eggoutflowTotal' 'larvacontent' 'larvaoutflowTotal' 'pupacontent' 'pupaoutflowTotal' 'adultcontent' 'adultoutflowTotal'};
%monat={'January' 'February' 'March' 'April' 'May' 'June' 'July' 'August' 'September' 'October' 'November' 'December'};

figure1 = figure('Color',[1 1 1]);
Color={'[1 0 0]' '[1 0 0]' '[1 1 0]' '[1 1 0]' '[0 1 1]' '[0 1 1]' '[1 0 1]' '[1 0 1]'} ;
for i=1:8
    %transfer data into variable d1 
    d1=cell2num(data(i));
    
    subplot1 = subplot(4,2,i,'Parent',figure1);
    plot(doy,eval(d1),'Color',eval(cell2num(Color(i))),'LineWidth',2); 
    ylim([0 109]);
    ylabel(['\fontsize{10}' data(i)]);
    xlabel(['\fontsize{10}' 'Time (day of year)' ]);
end   
     clear d1;
     
     
%% PLOT2
figure2 = figure('Color',[1 1 1]);
x=find(doy<9.5*30.5 & doy>5*30.5);
plot(doy(x),Tavg(x),'Color',[1 0 0],'LineWidth',2);
%ylim([0 25);
ylabel(['\fontsize{14}' 'Mean Temperature ^oC']);
xlabel(['\fontsize{14}' 'Time (day of year)' ]);
clear x;


%% PLOT3
figure3 = figure('Color',[1 1 1]);
hold
for i=1:4
    d1=cell2num(data(i*2-1));
    subplot1=subplot(1,2,1,'Parent',figure3);
    hold(subplot1,'on');
    plot(doy,eval(d1),'LineWidth',2)
    ylim([0 109]);
    xlabel(['\fontsize{10}' 'Time (day of year)' ]);
    %legend('Location','NorthEastOutside')
    legend(char(data(1)),char(data(3)),char(data(5)),char(data(7)));
end

for i=1:4
    d1=cell2num(data(i*2));
    subplot2=subplot(1,2,2,'Parent',figure3);
    hold(subplot2,'on');
    plot(doy,eval(d1),'LineWidth',2)
    ylim([0 109]);
    xlabel(['\fontsize{10}' 'Time (day of year)' ]);
    %legend('Location','NorthEastOutside')
    legend(char(data(2)),char(data(4)),char(data(6)),char(data(8)));
end





